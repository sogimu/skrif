#include "interpreter.h"
#include "compiler.h"
#include "vm.h"
#include "EnvScope.h"
#include "lexical_tokens.h"
#include "abstract_syntax_tree.h"
#include "disassembler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>

#include <cstdlib>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>

static bool g_print_bytecode = false;
static bool g_print_ast = false;

static const char HIST_NL = '\x1E';

static std::vector<std::string> g_ml_lines;
static int g_ml_index = -1;

bool ensureDirExists(const std::string& path)
{
    struct stat st{};
    if (stat(path.c_str(), &st) != 0) {
        if (mkdir(path.c_str(), 0700) != 0) {
            std::cerr << "Dir creation error " << path << ": " << strerror(errno) << "\n";
            return false;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        std::cerr << path << " exist but is not dir!\n";
        return false;
    }
    return true;
}

bool isInputComplete(const std::string& input)
{
    int balance = 0;
    bool in_string = false;
    char string_char = '0';

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (in_string) {
            if (c == string_char && (i == 0 || input[i - 1] != '\\'))
                in_string = false;
            continue;
        }
        if (c == '\'' || c == '"') { in_string = true; string_char = c; continue; }
        if (c == '{') balance++;
        else if (c == '}') balance--;
    }
    return balance <= 0;
}

static std::string g_history_file;
static void save_history_file(const std::string& path);

static Json eval_with_opts(const std::string& code)
{
    LexicalTokens lex(code);
    AbstractSyntaxTree ast(lex);

    if (g_print_ast)
        std::cout << ast.to_string() << std::endl;

    auto compiler = std::make_shared<Compiler>();
    Chunk chunk = compiler->compile(ast.root());

    if (g_print_bytecode)
        disassemble(std::cout, chunk, "<main>");

    auto env = std::make_shared<EnvScope>();
    VM vm;
    return vm.run(chunk, env);
}

static void commit_and_run(const std::string& code)
{
    add_history(code.c_str());
    save_history_file(g_history_file);

    try {
        auto result = eval_with_opts(code);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

static void ml_clear_display()
{
    fprintf(rl_outstream, "\r");
    for (int i = 0; i < g_ml_index; i++)
        fprintf(rl_outstream, "\033[A");
    fprintf(rl_outstream, "\033[J");
    rl_on_new_line();
}

static void ml_show_line(int idx)
{
    fprintf(rl_outstream, "\r\033[K");
    for (int i = 0; i < idx; i++)
        fprintf(rl_outstream, "%s%s\n", i == 0 ? ">>> " : "... ", g_ml_lines[i].c_str());
    g_ml_index = idx;
    rl_on_new_line();
    rl_set_prompt(idx == 0 ? ">>> " : "... ");
    rl_replace_line(g_ml_lines[idx].c_str(), 1);
    rl_point = rl_end;
    rl_redisplay();

    int below = (int)g_ml_lines.size() - 1 - idx;
    if (below > 0) {
        for (int i = idx + 1; i < (int)g_ml_lines.size(); i++)
            fprintf(rl_outstream, "\n... %s", g_ml_lines[i].c_str());
        for (int i = 0; i < below; i++)
            fprintf(rl_outstream, "\033[A");
        int col = 4 + (int)g_ml_lines[idx].size();
        fprintf(rl_outstream, "\r\033[%dC", col);
        fflush(rl_outstream);
    }
}

static int navigate_history(int dir)
{
    if (g_ml_index >= 0) {
        g_ml_lines[g_ml_index] = rl_line_buffer;
        int new_idx = g_ml_index + dir;
        if (new_idx >= 0 && new_idx < (int)g_ml_lines.size()) {
            ml_clear_display();
            ml_show_line(new_idx);
            return 0;
        }
        ml_clear_display();
        g_ml_index = -1;
        g_ml_lines.clear();
    }

    HIST_ENTRY* entry = (dir < 0) ? previous_history() : next_history();
    if (!entry) { rl_ding(); return 0; }

    std::string text = entry->line;
    if (text.find('\n') != std::string::npos) {
        std::istringstream ss(text);
        std::string ln;
        g_ml_lines.clear();
        while (std::getline(ss, ln)) g_ml_lines.push_back(ln);
        int show_idx = (dir < 0) ? 0 : (int)g_ml_lines.size() - 1;
        ml_show_line(show_idx);
    } else {
        g_ml_index = -1;
        g_ml_lines.clear();
        rl_replace_line(text.c_str(), 1);
        rl_set_prompt(">>> ");
        rl_point = rl_end;
        rl_redisplay();
    }
    return 0;
}

static int prev_history_fn(int, int) { return navigate_history(-1); }
static int next_history_fn(int, int) { return navigate_history(+1); }

static void save_history_file(const std::string& path)
{
    HIST_ENTRY** list = history_list();
    if (!list) return;
    std::ofstream f(path);
    for (int i = 0; list[i]; i++) {
        std::string entry = list[i]->line;
        for (auto& c : entry) if (c == '\n') c = HIST_NL;
        f << entry << "\n";
    }
}

static void load_history_file(const std::string& path)
{
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line)) {
        for (auto& c : line) if (c == HIST_NL) c = '\n';
        if (!line.empty()) add_history(line.c_str());
    }
}

int runFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Error: cannot open file '" << path << "': " << strerror(errno) << "\n";
        return 1;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    try {
        auto result = eval_with_opts(ss.str());
        if (!g_print_bytecode && !g_print_ast)
            std::cout << result << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    std::string file_arg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--print-bytecode")
            g_print_bytecode = true;
        else if (arg == "--print-ast")
            g_print_ast = true;
        else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [--print-bytecode] [--print-ast] [file.js]\n";
            std::cout << "\nREPL Key Bindings:\n";
            std::cout << "  Up Arrow      Navigate to previous history entry\n";
            std::cout << "  Down Arrow    Navigate to next history entry\n";
            std::cout << "  Ctrl+D        Exit REPL\n";
            std::cout << "  Ctrl+C        Exit REPL\n";
            std::cout << "  Enter         Submit input (or continue to next line if braces are unbalanced)\n";
            return 0;
        }
        else if (file_arg.empty())
            file_arg = arg;
        else {
            std::cerr << "Usage: " << argv[0] << " [--print-bytecode] [--print-ast] [file.js]\n";
            return 1;
        }
    }

    if (!file_arg.empty())
        return runFile(file_arg);

    std::cout << "Bytecode VM REPL. Enter Skrif code:\n";
    std::cout << "Multiline input supported (open braces continue to next line).\n";
    std::cout << "Ctrl+D or Ctrl+C to exit.\n";

    const char* home = getenv("HOME");
    if (!home) { std::cerr << "HOME not set\n"; return 1; }

    std::string config_dir = std::string(home) + "/.config/bytecode_vm_interpreter_repl";
    std::string history_file = config_dir + "/history";

    if (!ensureDirExists(config_dir)) return 1;

    g_history_file = history_file;
    if (access(history_file.c_str(), F_OK) == 0)
        load_history_file(history_file);

    rl_bind_keyseq("\033[A", prev_history_fn);
    rl_bind_keyseq("\033OA", prev_history_fn);
    rl_bind_keyseq("\033[B", next_history_fn);
    rl_bind_keyseq("\033OB", next_history_fn);

    std::string buffer;
    while (true) {
        const char* prompt = buffer.empty() ? ">>> " : "... ";
        char* line_cstr = readline(prompt);
        if (!line_cstr) { std::cout << "\nREPL exit.\n"; break; }

        std::string line(line_cstr);
        free(line_cstr);

        if (g_ml_index >= 0) {
            g_ml_lines[g_ml_index] = line;
            if (g_ml_index < (int)g_ml_lines.size() - 1)
                fprintf(stdout, "\r\033[J");
            std::string entry;
            for (size_t i = 0; i < g_ml_lines.size(); i++) {
                if (i > 0) entry += "\n";
                entry += g_ml_lines[i];
            }
            g_ml_index = -1;
            g_ml_lines.clear();
            buffer.clear();
            commit_and_run(entry);
            continue;
        }

        if (line.empty()) {
            if (!buffer.empty()) { commit_and_run(buffer); buffer.clear(); }
            continue;
        }

        bool starting_fresh = buffer.empty();
        if (!buffer.empty()) buffer += "\n";
        buffer += line;

        if (starting_fresh && isInputComplete(buffer)) {
            commit_and_run(buffer);
            buffer.clear();
        }
    }

    save_history_file(history_file);
    return 0;
}
