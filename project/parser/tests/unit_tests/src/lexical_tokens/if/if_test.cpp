#include "lexical_tokens.h"

#include <gtest/gtest.h>

using namespace std;

TEST( LEXICAL_TOKENS_IF, DEFAULT )
{
   // ARRANGE
   const auto& input = R"""(
    if(1){}else{}
  )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );

   // ASSERT
   LexicalTokens::LexicalToken t0{ .text="", .type=Token_Type::BOL, .line=0, .col=0, .length=0 };
   EXPECT_EQ( lexical_tokens[0], t0 );
   LexicalTokens::LexicalToken t1{ .text="if", .type=Token_Type::IF, .line=1, .col=4, .length=2 };
   EXPECT_EQ( lexical_tokens[1], t1 );
   LexicalTokens::LexicalToken t2{ .text="(", .type=Token_Type::OPEN_CIRCLE_BRACKET, .line=1, .col=6, .length=1 };
   EXPECT_EQ( lexical_tokens[2], t2 );
   LexicalTokens::LexicalToken t3{ .text="1", .type=Token_Type::INT, .line=1, .col=7, .length=1 };
   EXPECT_EQ( lexical_tokens[3], t3 );
   LexicalTokens::LexicalToken t4{ .text=")", .type=Token_Type::CLOSE_CIRCLE_BRACKET, .line=1, .col=8, .length=1 };
   EXPECT_EQ( lexical_tokens[4], t4 );
   LexicalTokens::LexicalToken t5{ .text="{", .type=Token_Type::OPEN_CURLY_BRACKET, .line=1, .col=9, .length=1 };
   EXPECT_EQ( lexical_tokens[5], t5 );
   LexicalTokens::LexicalToken t6{ .text="}", .type=Token_Type::CLOSE_CURLY_BRACKET, .line=1, .col=10, .length=1 };
   EXPECT_EQ( lexical_tokens[6], t6 );
   LexicalTokens::LexicalToken t7{ .text="else", .type=Token_Type::ELSE, .line=1, .col=11, .length=4 };
   EXPECT_EQ( lexical_tokens[7], t7 );
   LexicalTokens::LexicalToken t8{ .text="{", .type=Token_Type::OPEN_CURLY_BRACKET, .line=1, .col=15, .length=1 };
   EXPECT_EQ( lexical_tokens[8], t8 );
   LexicalTokens::LexicalToken t9{ .text="}", .type=Token_Type::CLOSE_CURLY_BRACKET, .line=1, .col=16, .length=1 };
   EXPECT_EQ( lexical_tokens[9], t9 );
   LexicalTokens::LexicalToken t10{ .text="", .type=Token_Type::EOL, .line=0, .col=0, .length=0 };
   EXPECT_EQ( lexical_tokens[10], t10 );
}

TEST( LEXICAL_TOKENS_IF, F_NOT_EQUAL_F )
{
   // ARRANGE
   const auto& input = R"""(
    if(1!=1){}else{}
  )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );

   // ASSERT
   LexicalTokens::LexicalToken t0{ .text="", .type=Token_Type::BOL, .line=0, .col=0, .length=0 };
   EXPECT_EQ( lexical_tokens[0], t0 );
   LexicalTokens::LexicalToken t1{ .text="if", .type=Token_Type::IF, .line=1, .col=4, .length=2 };
   EXPECT_EQ( lexical_tokens[1], t1 );
   LexicalTokens::LexicalToken t2{ .text="(", .type=Token_Type::OPEN_CIRCLE_BRACKET, .line=1, .col=6, .length=1 };
   EXPECT_EQ( lexical_tokens[2], t2 );
   LexicalTokens::LexicalToken t3{ .text="1", .type=Token_Type::INT, .line=1, .col=7, .length=1 };
   EXPECT_EQ( lexical_tokens[3], t3 );
   LexicalTokens::LexicalToken t4{ .text="!", .type=Token_Type::NOT, .line=1, .col=8, .length=1 };
   EXPECT_EQ( lexical_tokens[4], t4 );
   LexicalTokens::LexicalToken t5{ .text="=", .type=Token_Type::EQUAL, .line=1, .col=9, .length=1 };
   EXPECT_EQ( lexical_tokens[5], t5 );
   LexicalTokens::LexicalToken t6{ .text="1", .type=Token_Type::INT, .line=1, .col=10, .length=1 };
   EXPECT_EQ( lexical_tokens[6], t6 );
   LexicalTokens::LexicalToken t7{ .text=")", .type=Token_Type::CLOSE_CIRCLE_BRACKET, .line=1, .col=11, .length=1 };
   EXPECT_EQ( lexical_tokens[7], t7 );
   LexicalTokens::LexicalToken t8{ .text="{", .type=Token_Type::OPEN_CURLY_BRACKET, .line=1, .col=12, .length=1 };
   EXPECT_EQ( lexical_tokens[8], t8 );
   LexicalTokens::LexicalToken t9{ .text="}", .type=Token_Type::CLOSE_CURLY_BRACKET, .line=1, .col=13, .length=1 };
   EXPECT_EQ( lexical_tokens[9], t9 );
   LexicalTokens::LexicalToken t10{ .text="else", .type=Token_Type::ELSE, .line=1, .col=14, .length=4 };
   EXPECT_EQ( lexical_tokens[10], t10 );
   LexicalTokens::LexicalToken t11{ .text="{", .type=Token_Type::OPEN_CURLY_BRACKET, .line=1, .col=18, .length=1 };
   EXPECT_EQ( lexical_tokens[11], t11 );
   LexicalTokens::LexicalToken t12{ .text="}", .type=Token_Type::CLOSE_CURLY_BRACKET, .line=1, .col=19, .length=1 };
   EXPECT_EQ( lexical_tokens[12], t12 );
   LexicalTokens::LexicalToken t13{ .text="", .type=Token_Type::EOL, .line=0, .col=0, .length=0 };
   EXPECT_EQ( lexical_tokens[13], t13 );
}
