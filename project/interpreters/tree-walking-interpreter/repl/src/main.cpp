#include "interpreter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>

#include <cstdlib>   // getenv
#include <sys/stat.h> // mkdir
#include <cstring>   // strerror
#include <cerrno>    // errno
#include <unistd.h>  // access

// Separator used in history file to encode embedded newlines.
static const char HIST_NL = '\x1E';

// Multiline history navigation state.
// When g_ml_index >= 0, the user is navigating within a recalled multiline entry.
// g_ml_lines holds all lines; g_ml_index is the line currently in the readline buffer.
static std::vector<std::string> g_ml_lines;
static int g_ml_index = -1;

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

static std::string g_history_file;
static void save_history_file( const std::string& path );  // forward declaration

static void commit_and_run( const std::string& code )
{
    add_history( code.c_str() );
    save_history_file( g_history_file );  // persist before running (survives crashes)

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

// Erase the full multiline display: g_ml_index static lines above + the readline line
// + any below-lines printed via DECSC/DECRC.  Leaves cursor at column 0, ready for
// rl_on_new_line() / rl_redisplay() to start fresh.
static void ml_clear_display()
{
    fprintf( rl_outstream, "\r" );
    for( int i = 0; i < g_ml_index; i++ )
        fprintf( rl_outstream, "\033[A" );
    fprintf( rl_outstream, "\033[J" );
    rl_on_new_line();
}

// Render the multiline entry with line idx in the editable readline buffer.
// Lines 0..idx-1 are printed as static text above; lines idx+1..n-1 are printed
// below and cursor is moved back up so readline owns the correct terminal row.
static void ml_show_line( int idx )
{
    // Erase the terminal line where readline currently sits.  Needed when entering
    // ML mode fresh (the old ">>> " prompt is still there); harmless after
    // ml_clear_display (the line is already blank).
    fprintf( rl_outstream, "\r\033[K" );

    for( int i = 0; i < idx; i++ )
        fprintf( rl_outstream, "%s%s\n", i == 0 ? ">>> " : "... ", g_ml_lines[i].c_str() );

    // Cursor is now at column 0 of the line where readline will draw.
    g_ml_index = idx;
    rl_on_new_line();
    rl_set_prompt( idx == 0 ? ">>> " : "... " );
    rl_replace_line( g_ml_lines[idx].c_str(), 1 );
    rl_point = rl_end;
    rl_redisplay();

    int below = (int)g_ml_lines.size() - 1 - idx;
    if( below > 0 )
    {
        for( int i = idx + 1; i < (int)g_ml_lines.size(); i++ )
            fprintf( rl_outstream, "\n... %s", g_ml_lines[i].c_str() );
        // Move cursor back up to the readline row and to the rl_end column.
        // Using \033[A instead of DECSC/DECRC so scrolling doesn't break position.
        for( int i = 0; i < below; i++ )
            fprintf( rl_outstream, "\033[A" );
        int col = 4 + (int)g_ml_lines[idx].size();  // prompt (4) + content length
        fprintf( rl_outstream, "\r\033[%dC", col );
        fflush( rl_outstream );
    }
}

static int navigate_history( int dir )
{
    if( g_ml_index >= 0 )
    {
        // Save any edits the user made to the active line.
        g_ml_lines[g_ml_index] = rl_line_buffer;

        int new_idx = g_ml_index + dir;
        if( new_idx >= 0 && new_idx < (int)g_ml_lines.size() )
        {
            // Move cursor between lines within the same entry.
            ml_clear_display();
            ml_show_line( new_idx );
            return 0;
        }

        // Reached the top or bottom boundary: exit ML mode and fall through to
        // navigate to the adjacent history entry.
        ml_clear_display();
        g_ml_index = -1;
        g_ml_lines.clear();
    }

    HIST_ENTRY* entry = ( dir < 0 ) ? previous_history() : next_history();
    if( !entry ) { rl_ding(); return 0; }

    std::string text = entry->line;

    if( text.find('\n') != std::string::npos )
    {
        std::istringstream ss( text );
        std::string ln;
        g_ml_lines.clear();
        while( std::getline(ss, ln) ) g_ml_lines.push_back(ln);

        // Up → cursor at top line; Down → cursor at bottom line.
        int show_idx = ( dir < 0 ) ? 0 : (int)g_ml_lines.size() - 1;
        ml_show_line( show_idx );
    }
    else
    {
        g_ml_index = -1;
        g_ml_lines.clear();
        rl_replace_line( text.c_str(), 1 );
        rl_set_prompt( ">>> " );
        rl_point = rl_end;
        rl_redisplay();
    }
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

int runFile( const std::string& path )
{
    std::ifstream file( path );
    if( !file )
    {
        std::cerr << "Error: cannot open file '" << path << "': " << strerror( errno ) << "\n";
        return 1;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    const std::string source = ss.str();

    try
    {
        Interpreter interpreter;
        auto result = interpreter.eval( source.c_str() );
        std::cout << result << "\n";
    }
    catch( const SyntaxException& e )
    {
        const auto& stack = e.stack();
        const auto& last_node = *stack.rbegin();
        const auto last_token = last_node->lexical_tokens().at(0);
        PointToSyntaxError( source, last_token.line, last_token.col );
        return 1;
    }
    return 0;
}

int main( int argc, char* argv[] )
{
    if( argc > 1 )
    {
        return runFile( argv[1] );
    }

    std::cout << "JavaScript AST REPL. Enter JavaScript code:\n";
    std::cout << "Multiline input is supported (open braces continue to next line).\n";
    std::cout << "Ctrl+D or Ctrl+C to exit.\n";

    const char* home = getenv("HOME");
    if( !home )
    {
        std::cerr << "Env HOME does not exist\n";
        return 1;
    }

    std::string config_dir = std::string(home) + "/.config/naive_stack_machine_repl";
    std::string history_file = config_dir + "/history";

    if( !ensureDirExists(config_dir) ) return 1;

    g_history_file = history_file;

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

        if( g_ml_index >= 0 )
        {
            // Enter pressed while navigating a multiline history entry.
            g_ml_lines[g_ml_index] = line;  // save any edits to the active line

            // Erase below-lines still visible on the terminal (cursor lands on the
            // first one after readline outputs its trailing newline).
            if( g_ml_index < (int)g_ml_lines.size() - 1 )
                fprintf( stdout, "\r\033[J" );

            std::string entry;
            for( size_t i = 0; i < g_ml_lines.size(); i++ )
            {
                if( i > 0 ) entry += "\n";
                entry += g_ml_lines[i];
            }
            g_ml_index = -1;
            g_ml_lines.clear();
            buffer.clear();
            commit_and_run( entry );
            continue;
        }

        if( line.empty() )
        {
            if( !buffer.empty() )
            {
                commit_and_run( buffer );
                buffer.clear();
            }
            continue;
        }

        bool starting_fresh = buffer.empty();
        if( !buffer.empty() ) buffer += "\n";
        buffer += line;

        if( starting_fresh && isInputComplete(buffer) )
        {
            commit_and_run( buffer );
            buffer.clear();
        }
    }

    save_history_file( history_file );
    return 0;
}
