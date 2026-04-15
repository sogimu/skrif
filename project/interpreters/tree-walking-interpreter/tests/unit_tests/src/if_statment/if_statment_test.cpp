#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, VARIBLE_LESS_INT )
{
   // ARRANGE
   const auto& input = R"""({var a = 231; if( a < 1000 ) { return 234; } })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 234 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, INT_LESS_INT )
{
   // ARRANGE
   const auto& input = R"""({if( 1 < 1000 ) { return 234; } })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 234 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, BIN_EXPR_LESS_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""({if( 1+2 < 1000+2000 ) { return 234; } })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 234 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, IF_ELSE_TRUE_BRANCH )
{
   // ARRANGE
   const auto& input = R"""({var a = 5; if( a > 3 ) { return 100; } else { return 200; } })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 100 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, IF_ELSE_FALSE_BRANCH )
{
   // ARRANGE
   const auto& input = R"""({var a = 1; if( a > 3 ) { return 100; } else { return 200; } })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 200 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, NESTED_IF_ELSE )
{
   // ARRANGE
   const auto& input = R"""({
      var a = 5;
      var b = 10;
      if( a > 3 ) {
         if( b > 8 ) {
            return 1;
         } else {
            return 2;
         }
      } else {
         return 3;
      }
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 1 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, IF_ELSE_WITH_VARIABLES )
{
   // ARRANGE
   const auto& input = R"""({
      var result = 0;
      var condition = 7;
      if( condition > 5 ) {
         result = condition * 2;
      } else {
         result = condition + 10;
      }
      return result;
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 14 );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, IF_ELSE_CHAIN )
{
   // ARRANGE
   const auto& input = R"""({
      var score = 85;
      if( score >= 90 ) {
         return "A";
      } else {
         if( score >= 80 ) {
            return "B";
         } else {
            if( score >= 70 ) {
               return "C";
            } else {
               return "F";
            }
         }
      }
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_string(), true );
   EXPECT_EQ( result.get_string(), "B" );
}

TEST( NAIVE_STACK_INTERPRETER_IF_STATMENT, IF_WITH_ARRAY_CONDITION )
{
   // ARRANGE
   const auto& input = R"""({
      var arr = [1, 2, 3];
      if( arr[1] > 1 ) {
         return arr[2];
      } else {
         return arr[0];
      }
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 3 );
}
