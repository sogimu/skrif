#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, NUMBER_INT )
{
   // ARRANGE
   const auto& input = R"""(1;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 1 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_ADDITION_INT )
{
   // ARRANGE
   const auto& input = R"""(1+2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 3 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_DIVIDE_INT )
{
   // ARRANGE
   const auto& input = R"""(1.0/2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 0.5 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_DIVIDE_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(1/2.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 0.5 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_DIVIDE_INT )
{
   // ARRANGE
   const auto& input = R"""(1/2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 0 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_MULTIPLY_INT )
{
   // ARRANGE
   const auto& input = R"""(2.0*2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 4.0 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_MULTIPLY_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(2*2.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 4.0 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_MULTIPLY_INT )
{
   // ARRANGE
   const auto& input = R"""(2*2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 4 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_EQUAL_INT )
{
   // ARRANGE
   const auto& input = R"""(2.0==2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_EQUAL_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(2==2.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_LESS_INT )
{
   // ARRANGE
   const auto& input = R"""(1.0<2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_LESS_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(1<2.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_MORE_INT )
{
   // ARRANGE
   const auto& input = R"""(2.0>1;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_MORE_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(2>1.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_LESS_OR_EQUAL_INT )
{
   // ARRANGE
   const auto& input = R"""(2.0<=2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_LESS_OR_EQUAL_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(2<=2.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_MORE_OR_EQUAL_INT )
{
   // ARRANGE
   const auto& input = R"""(2.0>=2;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_INT_MORE_OR_EQUAL_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(2>=2.0;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_bool(), true );
   EXPECT_EQ( result.get_bool(), true );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, BIN_EXPR_DOUBLE_ADDITION_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""(1.5+2.3;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 3.8 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, RETURN_INT )
{
   // ARRANGE
   const auto& input = R"""(return 1;)""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 1 );
}

TEST( NAIVE_STACK_INTERPRETER_ARITHMETIC, FUNCTION_CALL_INT )
{
   // ARRANGE
   const auto& input = R"""({ function foo(arg){ return arg+1; } foo(1); } )""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 2 );
}
