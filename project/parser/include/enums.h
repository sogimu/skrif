#pragma once

#include <ostream>
#include <cassert>

enum class Token_Type
{
   BOL,
   EOL,
   DOUBLE,
   INT,
   PLUS,
   MINUS,
   ASTERISK,
   SLASH,
   OPEN_CIRCLE_BRACKET,
   CLOSE_CIRCLE_BRACKET,
   SEMICOLON,
   COLON,
   RETURN,
   RETURN_STATMENT,
   EQUAL,
   LESS,
   MORE,
   PRINT,
   ADDITION,
   SUBTRACTION,
   MULTIPLY,
   DIVISION,
   UnExpr,
   BinExpr,
   MEMBER_EXPRESSION,
   IF,
   ELSE,
   WHILE,
   GOTO,
   F,
   ASSIGMENT,
   NAME,
   COMMA,
   STATMENT,
   COMPUTATIONAL_EXPRESSION,
   OPEN_CURLY_BRACKET,
   CLOSE_CURLY_BRACKET,
   OPEN_SQUARE_BRACKET,
   CLOSE_SQUARE_BRACKET,
   SCOPE_STATMENT,
   FUNCTION_SCOPE_STATMENT,
   ARRAY,
   OBJECT,
   OBJECT_PAIR,
   CONDITIONAL_EXPRESSION,
   LESS_CONDITIONAL_EXPRESSION,
   MORE_CONDITIONAL_EXPRESSION,
   PRINT_STATMENT,
   IF_STATMENT,
   WHILE_STATMENT,
   FUNCTION,
   FUNCTION_STATMENT,
   FUNCTION_CALL,
   VARIBLE,
   VARIBLE_ASSIGMENT_STATMENT,
   VAR,
   STRING,
   NO
};

inline std::ostream& operator<<(std::ostream& os, Token_Type type) {
    switch(type) {
        case Token_Type::BOL: return os << "BOL";
        case Token_Type::EOL: return os << "EOL";
        case Token_Type::DOUBLE: return os << "DOUBLE";
        case Token_Type::INT: return os << "INT";
        case Token_Type::PLUS: return os << "PLUS";
        case Token_Type::MINUS: return os << "MINUS";
        case Token_Type::ASTERISK: return os << "ASTERISK";
        case Token_Type::SLASH: return os << "SLASH";
        case Token_Type::OPEN_CIRCLE_BRACKET: return os << "OPEN_CIRCLE_BRACKET";
        case Token_Type::CLOSE_CIRCLE_BRACKET: return os << "CLOSE_CIRCLE_BRACKET";
        case Token_Type::SEMICOLON: return os << "SEMICOLON";
        case Token_Type::COLON: return os << "COLON";
        case Token_Type::RETURN: return os << "RETURN";
        case Token_Type::RETURN_STATMENT: return os << "RETURN_STATMENT";
        case Token_Type::EQUAL: return os << "EQUAL";
        case Token_Type::LESS: return os << "LESS";
        case Token_Type::MORE: return os << "MORE";
        case Token_Type::PRINT: return os << "PRINT";
        case Token_Type::ADDITION: return os << "ADDITION";
        case Token_Type::SUBTRACTION: return os << "SUBTRACTION";
        case Token_Type::MULTIPLY: return os << "MULTIPLY";
        case Token_Type::DIVISION: return os << "DIVISION";
        case Token_Type::UnExpr: return os << "UnExpr";
        case Token_Type::BinExpr: return os << "BinExpr";
        case Token_Type::MEMBER_EXPRESSION: return os << "MEMBER_EXPRESSION";
        case Token_Type::IF: return os << "IF";
        case Token_Type::ELSE: return os << "ELSE";
        case Token_Type::WHILE: return os << "WHILE";
        case Token_Type::GOTO: return os << "GOTO";
        case Token_Type::F: return os << "F";
        case Token_Type::ASSIGMENT: return os << "ASSIGMENT";
        case Token_Type::NAME: return os << "NAME";
        case Token_Type::COMMA: return os << "COMMA";
        case Token_Type::STATMENT: return os << "STATMENT";
        case Token_Type::COMPUTATIONAL_EXPRESSION: return os << "COMPUTATIONAL_EXPRESSION";
        case Token_Type::OPEN_CURLY_BRACKET: return os << "OPEN_CURLY_BRACKET";
        case Token_Type::CLOSE_CURLY_BRACKET: return os << "CLOSE_CURLY_BRACKET";
        case Token_Type::OPEN_SQUARE_BRACKET: return os << "OPEN_SQUARE_BRACKET";
        case Token_Type::CLOSE_SQUARE_BRACKET: return os << "CLOSE_SQUARE_BRACKET";
        case Token_Type::SCOPE_STATMENT: return os << "SCOPE_STATMENT";
        case Token_Type::FUNCTION_SCOPE_STATMENT: return os << "FUNCTION_SCOPE_STATMENT";
        case Token_Type::ARRAY: return os << "ARRAY";
        case Token_Type::OBJECT: return os << "OBJECT";
        case Token_Type::CONDITIONAL_EXPRESSION: return os << "CONDITIONAL_EXPRESSION";
        case Token_Type::LESS_CONDITIONAL_EXPRESSION: return os << "LESS_CONDITIONAL_EXPRESSION";
        case Token_Type::MORE_CONDITIONAL_EXPRESSION: return os << "MORE_CONDITIONAL_EXPRESSION";
        case Token_Type::PRINT_STATMENT: return os << "PRINT_STATMENT";
        case Token_Type::IF_STATMENT: return os << "IF_STATMENT";
        case Token_Type::WHILE_STATMENT: return os << "WHILE_STATMENT";
        case Token_Type::FUNCTION: return os << "FUNCTION";
        case Token_Type::FUNCTION_STATMENT: return os << "FUNCTION_STATMENT";
        case Token_Type::FUNCTION_CALL: return os << "FUNCTION_CALL";
        case Token_Type::VARIBLE: return os << "VARIBLE";
        case Token_Type::VARIBLE_ASSIGMENT_STATMENT: return os << "VARIBLE_ASSIGMENT_STATMENT";
        case Token_Type::VAR: return os << "VAR";
        case Token_Type::STRING: return os << "STRING";
        case Token_Type::NO: return os << "NO";
        default: 
            assert(false && "Неизвестный тип токена");
            return os << "Unknown Token Type";
    }
    return os;
}
