#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( NAIVE_STACK_INTERPRETER_WHILE_STATMENT, SIMPLE_COUNTER )
{
    // ARRANGE
    const auto& input = R"""({ var counter = 0; while( counter < 5 ) { counter = counter + 1; } return counter; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 5 );
}

TEST( NAIVE_STACK_INTERPRETER_WHILE_STATMENT, ACCUMULATOR )
{
    // ARRANGE
    const auto& input = R"""({ var sum = 0; var i = 1; while( i <= 10 ) { sum = sum + i; i = i + 1; } return sum; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 55 );
}

TEST( NAIVE_STACK_INTERPRETER_WHILE_STATMENT, FACTORIAL )
{
    // ARRANGE
    const auto& input = R"""({ var n = 5; var fact = 1; var i = 1; while( i <= n ) { fact = fact * i; i = i + 1; } return fact; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 120 );
}

TEST( NAIVE_STACK_INTERPRETER_WHILE_STATMENT, NESTED_LOOP )
{
    // ARRANGE
    const auto& input = R"""({ 
        var total = 0; 
        var i = 1; 
        while( i <= 3 ) { 
            var j = 1; 
            while( j <= 2 ) { 
                total = total + 1; 
                j = j + 1; 
            } 
            i = i + 1; 
        } 
        return total; 
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 6 );
}

TEST( NAIVE_STACK_INTERPRETER_WHILE_STATMENT, BREAK_CONDITION_FALSE )
{
    // ARRANGE
    const auto& input = R"""({ var i = 10; while( i < 5 ) { i = i + 1; } return i; })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 10 );
}

TEST( NAIVE_STACK_INTERPRETER_WHILE_STATMENT, WITH_ARRAY_MODIFICATION )
{
    // ARRANGE
    const auto& input = R"""({ 
        var arr = [1, 2, 3]; 
        var i = 0; 
        while( i < 3 ) { 
            arr[i] = arr[i] * 2; 
            i = i + 1; 
        } 
        return arr; 
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_array(), true );
    std::vector<Json> expected_array{ 2, 4, 6 };
    EXPECT_EQ( result.get_array(), expected_array );
}
