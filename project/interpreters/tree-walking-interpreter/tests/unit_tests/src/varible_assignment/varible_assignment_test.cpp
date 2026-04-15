#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, LOCAL_READ_WRITE_INT )
{
   // ARRANGE
   const auto& input = R"""({ var a = 1; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 1 );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, LOCAL_READ_WRITE_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""({ var a = 1.5; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 1.5 );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, LOCAL_READ_WRITE_STRING )
{
   // ARRANGE
   const auto& input = R"""({ var a = "some string"; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_string(), true );
   EXPECT_EQ( result.get_string(), "some string" );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, LOCAL_READ_WRITE_ARRAY )
{
   // ARRANGE
   const auto& input = R"""({ var a = [1,2,3]; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_array(), true );
   std::vector<Json> expected_array{ 1, 2, 3};
   EXPECT_EQ( result.get_array(), expected_array );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, LOCAL_READ_WRITE_OBJECT )
{
   // ARRANGE
   const auto& input = R"""({ var a = {"a": 1, "b": 2}; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_object(), true );
   std::map<std::string, Json> expected_object{ {"a", 1}, {"b", 2} };
   EXPECT_EQ( result.get_object(), expected_object );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, GLOBAL_READ_WRITE_INT )
{
   // ARRANGE
   const auto& input = R"""({ a = 1; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_int(), true );
   EXPECT_EQ( result.get_int(), 1 );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, GLOBAL_READ_WRITE_DOUBLE )
{
   // ARRANGE
   const auto& input = R"""({ a = 1.5; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_double(), true );
   EXPECT_EQ( result.get_double(), 1.5 );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, GLOBAL_READ_WRITE_STRING )
{
   // ARRANGE
   const auto& input = R"""({ a = "some string"; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_string(), true );
   EXPECT_EQ( result.get_string(), "some string" );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, GLOBAL_READ_WRITE_ARRAY )
{
   // ARRANGE
   const auto& input = R"""({ a = [1,2,3]; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_array(), true );
   std::vector<Json> expected_array{ 1, 2, 3};
   EXPECT_EQ( result.get_array(), expected_array );
}

TEST( NAIVE_STACK_INTERPRETER_VARIBLE_ASSIGNMENT, GLOBAL_READ_WRITE_OBJECT )
{
   // ARRANGE
   const auto& input = R"""({ a = {"a": 1, "b": 2}; return a; })""";

   // ACT
   Interpreter naive_stack_machine;
   auto result = naive_stack_machine.eval( input );

   // ASSERT
   EXPECT_EQ( result.is_object(), true );
   std::map<std::string, Json> expected_object{ {"a", 1}, {"b", 2} };
   EXPECT_EQ( result.get_object(), expected_object );
}

