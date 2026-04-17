#include "interpreter.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>

// Separator used in history file to encode embedded newlines.
static const char HIST_NL = '\x1E';

// Pending multiline entry recalled from history (waiting for Enter).
static std::string g_pending_entry;
static std::string g_pending_first_line;

bool ensureDirExists( const std::string& path )
{
    struct stat st{};
    if (stat(path.c_str(), &st) != 0) {
        if (mkdir(path.c_str(), 0700) != 0) {
            std::cerr << "Dir creation error " << path << ": " << strerror( errno ) << "\n";
            return false;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        std::cerr << path << " exist but is not dir!\n";
        return false;
    }
    return true;
}

bool isInputComplete( const std::string& input )
{
    int balance = 0;
    bool in_string = false;
    char string_char = '0';

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (in_string) {
            if (c == string_char && input[i - 1] != '\\') {
                in_string = false;
            }
            continue;
        } else {
            if (c == '\'' || c == '"') {
                in_string = true;
                string_char = c;
                continue;
            }
        }

        if (c == '{') balance++;
        else if (c == '}') balance--;
    }

    return balance <= 0;
}

void PointToSyntaxError( const std::string& text, int line, int col )
{
  stringstream ss{ text };
  std::string currentLine;
  int currentLineNumber = 0;
  bool markerPlaced = false;

  std::cout << "File \"<stdin>\", line " << std::to_string( line ) << std::endl;
  while( getline(ss, currentLine) )
  {
      if (currentLineNumber == line && !markerPlaced)
      {
          std::cout << currentLine << std::endl;
          if (col <= currentLine.size())
          {
              std::cout << string(col, ' ') << "^" << std::endl;
              std::cout << "SyntaxError: invalid syntax" << std::endl;
              markerPlaced = true;
          }
      }
      currentLineNumber++;
  }
  if( !markerPlaced && line < currentLineNumber )
  {
      std::cout << string(col, ' ') << "^" << endl;
      std::cout << "SyntaxError: invalid syntax" << std::endl;
  }
}

static void execute_code( const std::string& code )
{
    try
    {
        Interpreter naive_stack_machine;
        auto result = naive_stack_machine.eval( code.c_str() );
        std::cout << result << std::endl;
    }
    catch( const SyntaxException& e )
    {
        const auto& stack = e.stack();
        const auto& last_node = *stack.rbegin();
        const auto last_token = last_node->lexical_tokens().at(0);
        PointToSyntaxError( code, last_token.line, last_token.col );
    }
}

// Print a multiline entry with >>> / ... prompts (above the readline input line).
static void print_entry_preview( const std::string& text )
{
    std::istringstream ss( text );
    std::string line;
    bool first = true;
    while( std::getline(ss, line) )
    {
        fprintf( rl_outstream, first ? ">>> %s\n" : "... %s\n", line.c_str() );
        first = false;
    }
}

static int navigate_history( int dir )
{
    HIST_ENTRY* entry = ( dir < 0 ) ? previous_history() : next_history();
    if( !entry ) { rl_ding(); return 0; }

    std::string text = entry->line;

    if( text.find('\n') != std::string::npos )
    {
        // Multiline entry: print formatted preview above the prompt, then show
        // only the first line in the readline buffer so the display stays clean.
        print_entry_preview( text );
        rl_on_new_line();
        g_pending_entry = text;
        g_pending_first_line = text.substr( 0, text.find('\n') );
        rl_replace_line( g_pending_first_line.c_str(), 1 );
    }
    else
    {
        g_pending_entry.clear();
        g_pending_first_line.clear();
        rl_replace_line( text.c_str(), 1 );
    }

    rl_point = rl_end;
    rl_redisplay();
    return 0;
}

static int prev_history_fn( int count, int key ) { return navigate_history( -1 ); }
static int next_history_fn( int count, int key ) { return navigate_history( +1 ); }

// History file format: one entry per line, embedded \n encoded as HIST_NL.
static void save_history_file( const std::string& path )
{
    HIST_ENTRY** list = history_list();
    if( !list ) return;
    std::ofstream f( path );
    for( int i = 0; list[i]; i++ )
    {
        std::string entry = list[i]->line;
        for( auto& c : entry ) if( c == '\n' ) c = HIST_NL;
        f << entry << "\n";
    }
}

static void load_history_file( const std::string& path )
{
    std::ifstream f( path );
    std::string line;
    while( std::getline(f, line) )
    {
        for( auto& c : line ) if( c == HIST_NL ) c = '\n';
        if( !line.empty() ) add_history( line.c_str() );
    }
}

int main()
{
    std::cout << "JavaScript AST REPL. Enter JavaScript code:\n";
    std::cout << "Multiline input is supported (open braces continue to next line).\n";
    std::cout << "Ctrl+D to exit and save history.\n";

    const char* home = getenv("HOME");
    if( !home )
    {
        std::cerr << "Env HOME does not exist\n";
        return 1;
    }

    std::string config_dir = std::string(home) + "/.config/naive_stack_machine_repl";
    std::string history_file = config_dir + "/history";

    if( !ensureDirExists(config_dir) ) return 1;

    if( access( history_file.c_str(), F_OK ) == 0 )
        load_history_file( history_file );

    rl_bind_keyseq( "\033[A", prev_history_fn );  // Up
    rl_bind_keyseq( "\033OA", prev_history_fn );  // Up (application mode)
    rl_bind_keyseq( "\033[B", next_history_fn );  // Down
    rl_bind_keyseq( "\033OB", next_history_fn );  // Down (application mode)

    std::string buffer;
    while( true )
    {
        const char* prompt = buffer.empty() ? ">>> " : "... ";
        char* line_cstr = readline( prompt );
        if( !line_cstr )
        {
            std::cout << "\nREPL exit.\n";
            break;
        }

        std::string line( line_cstr );
        free( line_cstr );

        // If the user pressed Enter on a recalled multiline history entry
        // (first line unchanged), execute the full snippet.
        if( !g_pending_entry.empty() && line == g_pending_first_line )
        {
            std::string entry = g_pending_entry;
            g_pending_entry.clear();
            g_pending_first_line.clear();
            buffer.clear();
            add_history( entry.c_str() );
            execute_code( entry );
            continue;
        }
        g_pending_entry.clear();
        g_pending_first_line.clear();

        if( line.empty() )
        {
            if( !buffer.empty() )
            {
                add_history( buffer.c_str() );
                execute_code( buffer );
                buffer.clear();
            }
            continue;
        }

        bool starting_fresh = buffer.empty();
        if( !buffer.empty() ) buffer += "\n";
        buffer += line;

        if( starting_fresh && isInputComplete(buffer) )
        {
            add_history( buffer.c_str() );
            execute_code( buffer );
            buffer.clear();
        }
    }

    save_history_file( history_file );
    return 0;
}
