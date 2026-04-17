#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, RETURN_INT )
{
   // ARRANGE
   const auto& input = R"""({ function foo() { return 234; } return foo(); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 234 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, CALL_WITH_INT )
{
   // ARRANGE
   const auto& input = R"""({ function foo(val) { return 234 + val; } return foo(1); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 235 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, CALL_WITH_INT_TWO_TIMES )
{
   // ARRANGE
   const auto& input = R"""({ function foo(val) { return 234 + val; } return foo(1) + foo(2); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 471 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, BIN_EXPR_LESS_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""({ var counter = 0; function foo(val) { print(val); counter = counter + 1; if( val > 0 ) { foo(val-1); } } foo(5); return counter; })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 6 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, MULTIPLE_PARAMETERS )
{
   // ARRANGE
   const auto& input = R"""({ function add(a, b, c) { return a + b + c; } return add(10, 20, 30); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 60 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, MEMBER_NOTATION_BRACKET_NO_ARGS )
{
   // ARRANGE
   const auto& input = R"""({ function greet() { return 42; }; var obj = {}; obj["fn"] = greet; return obj["fn"](); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 42 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, MEMBER_NOTATION_DOT_NO_ARGS )
{
   // ARRANGE
   const auto& input = R"""({ function greet() { return 42; }; var obj = {}; obj.fn = greet; return obj.fn(); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 42 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, MEMBER_NOTATION_BRACKET_WITH_ARGS )
{
   // ARRANGE
   const auto& input = R"""({ function add(a, b) { return a + b; }; var obj = {}; obj["fn"] = add; return obj["fn"](10, 20); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 30 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, MEMBER_NOTATION_DOT_WITH_ARGS )
{
   // ARRANGE
   const auto& input = R"""({ function add(a, b) { return a + b; }; var obj = {}; obj.fn = add; return obj.fn(10, 20); })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 30 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, LOCAL_VARIABLE_SCOPE )
{
   // ARRANGE
   const auto& input = R"""({
      var global_var = 100;
      function test() {
         var local_var = 50;
         return local_var + global_var;
      }
      return test();
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 150 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, GLOBAL_VARIABLE_MODIFICATION )
{
   // ARRANGE
   const auto& input = R"""({
      global_counter = 0;
      function increment() {
         global_counter = global_counter + 1;
         return global_counter;
      }
      increment();
      increment();
      return increment();
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 3 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, LOCAL_VARIABLE_SHADOW_GLOBAL )
{
   // ARRANGE
   const auto& input = R"""({
      var x = 100;
      function test() {
         var x = 50;
         return x;
      }
      return test();
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 50 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, NESTED_FUNCTIONS )
{
   // ARRANGE
   const auto& input = R"""({
      function outer(x) {
         function inner(y) {
            return x + y;
         }
         return inner(10);
      }
      return outer(5);
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 15 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, FUNCTION_RETURNING_FUNCTION )
{
   // ARRANGE
   const auto& input = R"""({
      function createMultiplier(factor) {
         function multiply(value) {
            return value * factor;
         }
         return multiply;
      }
      var doubler = createMultiplier(2);
      return doubler(5);
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 10 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, PARAMETER_SCOPE_ISOLATION )
{
   // ARRANGE
   const auto& input = R"""({
      var global_val = 100;
      function test(global_val) {
         return global_val + 1;
      }
      return test(50);
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 51 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, RECURSIVE_FIBONACCI )
{
   // ARRANGE
   const auto& input = R"""({
      function fib(n) {
         if( n <= 1 ) {
            return n;
         } else {
            return fib(n - 1) + fib(n - 2);
         }
      }
      return fib(7);
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 13 );
}

TEST( TREE_WALKING_INTERPRETER_FUNCTION_CALL, FUNCTION_WITH_ARRAY_PARAMETER )
{
   // ARRANGE
   const auto& input = R"""({
      function sum_array(arr) {
         var total = 0;
         var i = 0;
         while( i < 3 ) {
            total = total + arr[i];
            i = i + 1;
         }
         return total;
      }
      return sum_array([10, 20, 30]);
   })""";

   // ACT
   Interpreter interpreter;
   auto result = interpreter.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 60 );
}
