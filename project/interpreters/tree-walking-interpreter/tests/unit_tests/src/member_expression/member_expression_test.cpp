#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, ARRAY_READ_INDEX )
{
    // ARRANGE
    const auto& input = R"""({ var arr = [10, 20, 30]; return arr[1]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 20 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, EMPTY_ARRAY_READ_INDEX )
{
    // ARRANGE
    const auto& input = R"""({ var arr = []; return arr[1]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_null(), true );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, ARRAY_WRITE_INDEX )
{
    // ARRANGE
    const auto& input = R"""({ var arr = [10, 20, 30]; arr[1] = 99; return arr[1]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 99 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, EMPTY_ARRAY_WRITE_INDEX )
{
    // ARRANGE
    const auto& input = R"""({ var arr = []; arr[1] = 42; return arr[1]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 42 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, ARRAY_MODIFY_AND_RETURN_ARRAY )
{
    // ARRANGE
    const auto& input = R"""({ var arr = [1, 2, 3]; arr[0] = 100; arr[2] = 300; return arr; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_array(), true );
    std::vector<Json> expected_array{ 100, 2, 300 };
    EXPECT_EQ( result.get_array(), expected_array );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, OBJECT_READ_PROPERTY )
{
    // ARRANGE
    const auto& input = R"""({ var obj = {"name": "John", "age": 30}; return obj["name"]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_string(), true );
    EXPECT_EQ( result.get_string(), "John" );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, OBJECT_WRITE_PROPERTY )
{
    // ARRANGE
    const auto& input = R"""({ var obj = {"name": "John", "age": 30}; obj["age"] = 35; return obj["age"]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 35 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, OBJECT_ADD_NEW_PROPERTY )
{
    // ARRANGE
    const auto& input = R"""({ var obj = {"name": "John"}; obj["city"] = "Moscow"; return obj["city"]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_string(), true );
    EXPECT_EQ( result.get_string(), "Moscow" );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, OBJECT_MODIFY_AND_RETURN_OBJECT )
{
    // ARRANGE
    const auto& input = R"""({ var obj = {"a": 1, "b": 2}; obj["a"] = 10; obj["c"] = 3; return obj; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_object(), true );
    std::map<std::string, Json> expected_object{ {"a", 10}, {"b", 2}, {"c", 3} };
    EXPECT_EQ( result.get_object(), expected_object );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, NESTED_ARRAY_ACCESS )
{
    // ARRANGE
    const auto& input = R"""({ var nested = [[1, 2], [3, 4]]; return nested[1][0]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 3 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, NESTED_OBJECT_ACCESS )
{
    // ARRANGE
    const auto& input = R"""({ var nested = {"user": {"name": "Alice", "info": {"age": 25}}}; return nested["user"]["info"]["age"]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 25 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, VARIABLE_INDEX_ACCESS )
{
    // ARRANGE
    const auto& input = R"""({ var arr = [10, 20, 30]; var index = 2; return arr[index]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 30 );
}

TEST( NAIVE_STACK_INTERPRETER_MEMBER_EXPRESSION, EXPRESSION_INDEX_ACCESS )
{
    // ARRANGE
    const auto& input = R"""({ var arr = [100, 200, 300, 400]; return arr[1 + 1]; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 300 );
}
