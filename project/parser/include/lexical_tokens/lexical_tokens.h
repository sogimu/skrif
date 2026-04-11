#pragma once

#include "enums.h"

#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class LexicalTokens
{
public:
   struct LexicalToken
   {
      std::string text;
      Token_Type type;
      size_t line=0;
      size_t col=0;
      size_t length=1;
      bool operator==( const LexicalToken& token ) const
      {
         return token.text == text && token.type == type && token.line == line && token.col == col && token.length == length;
      }
   };

   LexicalTokens( const std::string& text );
   LexicalTokens( const std::vector< LexicalToken >& tokens );
   std::vector< LexicalToken > list() const;
   LexicalToken operator[]( size_t index ) const;
   bool operator==( const LexicalTokens& lexical_tokens ) const;

private:
   std::vector< LexicalToken > tokens;

   const std::set< char > didgits{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

   const std::set< char > alphabet{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_',
                                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

   const std::set< char > terminals{ '+', '-', '/', '*', '(', ')', '=', '<', '>', ';', ':', '{', '}', '[', ']', ',', '.', '!' };

   const std::unordered_map< std::string, Token_Type > token_by_text{
      { "if", Token_Type::IF }, 
      { "else", Token_Type::ELSE }, 
      { "while", Token_Type::WHILE }, 
      { "print", Token_Type::PRINT }, 
      { "return", Token_Type::RETURN }, 
      { "function", Token_Type::FUNCTION },
      { "var", Token_Type::VAR }
   };
};

inline std::ostream& operator<<( std::ostream& o, const LexicalTokens::LexicalToken& rhs )
{
   o << "[" << "\"" << rhs.text << "\"" << ", type=" << rhs.type << ", line=" << std::to_string(rhs.line) << ", col=" << std::to_string(rhs.col) << ", length=" << std::to_string(rhs.length) << "]";
   return o;
}

inline std::ostream& operator<<( std::ostream& o, const LexicalTokens& lexical_tokens )
{
  for( const auto& lexical_token : lexical_tokens.list() )
  {
      o << lexical_token << std::endl;
  }
  return o;
}
