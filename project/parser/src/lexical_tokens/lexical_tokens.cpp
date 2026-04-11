#include "lexical_tokens.h"


LexicalTokens::LexicalTokens( const std::string& text )
{
  std::vector< LexicalToken > result;
  result.emplace_back( LexicalToken{ "", Token_Type::BOL, 0, 0, 0 } );

  size_t line_index = 0;
  size_t col_index = 0;
  
  auto create_expression_generator = [&line_index, &col_index]( const auto& text )
  {
      struct CharInfo
      {
        char value;
        size_t line = 0;
        size_t col = 0;
        size_t length=1;
      };
     auto it = text.begin();
     auto sentinel = text.end();
     auto next_char = [ it, sentinel, &line_index, &col_index ]() mutable -> std::optional< CharInfo >
     {
        if( it == sentinel )
           return {};
        auto current_it = it++;
        auto char_value = *current_it;
        auto current_col_index = col_index++;
        auto current_line_index = line_index;
        if( char_value == '\n' )
        {
           ++line_index;
           col_index = 0;
        }
        return CharInfo{ .value=char_value, 
                         .line=current_line_index, 
                         .col=current_col_index, 
                         .length=1 };
     };

     struct Chars
     {
        std::optional< CharInfo > current;
        std::optional< CharInfo > next;
     };

     
     Chars chars;
     chars.next = next_char();

     return [ next_char, chars ]() mutable -> Chars
     {
         chars.current = chars.next;
         chars.next = next_char();

         auto c0 = chars.current;
         auto n0 = chars.next;
        return chars;
     };
  };

  auto next_chars = create_expression_generator( text );
  auto chars = next_chars();

  bool is_comment_state = false;
  while( chars.current.has_value() )
  {
     if( chars.current.value().value == '/' && chars.next.value().value == '/' )
     {
        is_comment_state = true;
     }

     if( is_comment_state )
     {
        if( chars.current.value().value == '\n' )
        {
           is_comment_state = false;
        }
        chars = next_chars();
     }
     else if( terminals.count( chars.current.value().value ) )
     {
        Token_Type terminal_token_type = Token_Type::NO;

        switch( chars.current.value().value )
        {
        case '+':
        {
           terminal_token_type = Token_Type::PLUS;
        };
        break;
        case '-':
        {
           terminal_token_type = Token_Type::MINUS;
        };
        break;
        case '*':
        {
           terminal_token_type = Token_Type::ASTERISK;
        };
        break;
        case '/':
        {
           terminal_token_type = Token_Type::SLASH;
        };
        break;
        case '(':
        {
           terminal_token_type = Token_Type::OPEN_CIRCLE_BRACKET;
        };
        break;
        case ')':
        {
           terminal_token_type = Token_Type::CLOSE_CIRCLE_BRACKET;
        };
        break;
        case '{':
        {
           terminal_token_type = Token_Type::OPEN_CURLY_BRACKET;
        };
        break;
        case '}':
        {
           terminal_token_type = Token_Type::CLOSE_CURLY_BRACKET;
        };
        break;
        case '[':
        {
           terminal_token_type = Token_Type::OPEN_SQUARE_BRACKET;
        };
        break;
        case ']':
        {
           terminal_token_type = Token_Type::CLOSE_SQUARE_BRACKET;
        };
        break;
        case '=':
        {
           terminal_token_type = Token_Type::EQUAL;
        };
        break;
        case '!':
        {
           terminal_token_type = Token_Type::NOT;
        };
        break;
        case '<':
        {
           terminal_token_type = Token_Type::LESS;
        };
        break;
        case '>':
        {
           terminal_token_type = Token_Type::MORE;
        };
        break;
        case ';':
        {
           terminal_token_type = Token_Type::SEMICOLON;
        };
        break;
        case ':':
        {
           terminal_token_type = Token_Type::COLON;
        };
        break;
        case ',':
        {
           terminal_token_type = Token_Type::COMMA;
        };
        break;

        default:
        {
        }
        }

        result.emplace_back( LexicalToken{ .text = std::string{ chars.current.value().value }, 
                                           .type = terminal_token_type, 
                                           .line = chars.current.value().line, 
                                           .col = chars.current.value().col } );
        chars = next_chars();
     }
     else if( didgits.count( chars.current.value().value ) )
     {
        std::string token;
        size_t line = chars.current.value().line;
        size_t col = chars.current.value().col;
        bool is_dot_met = false;
        for( ; chars.current && ( didgits.count( chars.current.value().value ) || ( !is_dot_met && chars.current.value().value == '.') ); chars = next_chars() )
        {
          token += chars.current.value().value;

          if( chars.current.value().value == '.' )
          {
            is_dot_met = true;
          }
        }
        if( is_dot_met )
        {
            result.emplace_back( LexicalToken{ .text = token, 
                                               .type = Token_Type::DOUBLE, 
                                               .line = line, 
                                               .col = col, 
                                               .length = token.size() } );
        }
        else
        {
            result.emplace_back( LexicalToken{ .text = token, 
                                               .type = Token_Type::INT, 
                                               .line = line, 
                                               .col = col, 
                                               .length = token.size() } );
        }
     }
     else if( chars.current.value().value == '"' )
     {
        std::string token;
        size_t line = chars.current.value().line;
        size_t col = chars.current.value().col;
        chars = next_chars();
        for( ; chars.current.value().value != '"'; chars = next_chars() )
        {
            token += chars.current.value().value;
        }
       
        chars = next_chars();

        LexicalToken lexical_token{ .text = token, 
                                    .type = Token_Type::STRING, 
                                    .line = line, 
                                    .col = col, 
                                    .length = token.size() };
        result.emplace_back( lexical_token );
     }
     else if( alphabet.count( chars.current.value().value ) )
     {
        std::string token;
        size_t line = chars.current.value().line;
        size_t col = chars.current.value().col;
        for( ; alphabet.count( chars.current.value().value ) || didgits.count( chars.current.value().value ); chars = next_chars() )
        {
           token += chars.current.value().value;
        }
        if( token_by_text.contains( token ) )
        {
            result.emplace_back( LexicalToken{ .text = token, 
                                               .type = token_by_text.at( token ), 
                                               .line = line, 
                                               .col = col , 
                                               .length = token.size() } );
        }
        else
        {
            LexicalToken lexical_token{ .text = token, 
                                        .type = Token_Type::NAME, 
                                        .line = line, 
                                        .col = col, 
                                        .length = token.size() };
            result.emplace_back( lexical_token );
        }
     }
     else
     {
        chars = next_chars();
     }
  }

  result.emplace_back( LexicalToken{ .text = "", 
                                     .type = Token_Type::EOL, 
                                     .line = 0,
                                     .col = 0,
                                     .length = 0 } );

  tokens = result;
}
LexicalTokens::LexicalTokens( const std::vector< LexicalToken >& tokens )
{
  this->tokens = tokens;
}

std::vector< LexicalTokens::LexicalToken > LexicalTokens::list() const
{
  return tokens;
}

LexicalTokens::LexicalToken LexicalTokens::operator[]( size_t index ) const
{
   return tokens.at(index);
}

bool LexicalTokens::operator==( const LexicalTokens& lexical_tokens ) const
{
  return list() == lexical_tokens.list();
}
