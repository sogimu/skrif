#include "interpreter.h"

#include <gtest/gtest.h>

using namespace std;

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, WHILE_WITH_IF_AND_FUNCTIONS )
{
    // ARRANGE
    const auto& input = R"""({
        function isEven(n) {
            return (n / 2) * 2 == n;
        }
        
        var sum = 0;
        var i = 1;
        while( i <= 10 ) {
            if( isEven(i) ) {
                sum = sum + i;
            }
            i = i + 1;
        }
        return sum;
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 30 ); // 2 + 4 + 6 + 8 + 10
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, ARRAY_PROCESSING_WITH_FUNCTIONS )
{
    // ARRANGE
    const auto& input = R"""({
        function processArray(arr) {
            var result = [];
            var i = 0;
            while( i < 3 ) {
                if( arr[i] > 5 ) {
                    result[i] = arr[i] * 2;
                } else {
                    result[i] = arr[i] + 10;
                }
                i = i + 1;
            }
            return result;
        }
        
        var input_array = [3, 7, 1];
        return processArray(input_array);
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_array(), true );
    std::vector<Json> expected_array{ 13, 14, 11 };  // [3+10, 7*2, 1+10]
    EXPECT_EQ( result.get_array(), expected_array );
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, OBJECT_MANIPULATION_WITH_LOOPS )
{
    // ARRANGE
    const auto& input = R"""({
        var person = {"name": "John", "age": 25, "score": 0};
        var i = 1;
        while( i <= 5 ) {
            person["score"] = person["score"] + i;
            i = i + 1;
        }
        return person["score"];
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 15 ); // 1+2+3+4+5
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, NESTED_FUNCTION_WITH_CLOSURE )
{
    // ARRANGE
    const auto& input = R"""({
        function createCounter(start) {
            var count = start;
            function increment() {
                count = count + 1;
                return count;
            }
            return increment;
        }
        
        var counter = createCounter(10);
        var result = [];
        var i = 0;
        while( i < 3 ) {
            result[i] = counter();
            i = i + 1;
        }
        return result;
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_array(), true );
    std::vector<Json> expected_array{ 11, 12, 13 };
    EXPECT_EQ( result.get_array(), expected_array );
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, RECURSIVE_TREE_TRAVERSAL )
{
    // ARRANGE
    const auto& input = R"""({
        function sumTree(node) {
            if( node["value"] == 0 ) {
                return 0;
            }
            var sum = node["value"];
            if( node["left"]["value"] != 0 ) {
                sum = sum + sumTree(node["left"]);
            }
            if( node["right"]["value"] != 0 ) {
                sum = sum + sumTree(node["right"]);
            }
            return sum;
        }
        
        var tree = {
            "value": 10,
            "left": {"value": 5, "left": {"value": 0}, "right": {"value": 0}},
            "right": {"value": 15, "left": {"value": 0}, "right": {"value": 0}}
        };
        
        return sumTree(tree);
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 30 ); // 10 + 5 + 15
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, MATRIX_OPERATIONS )
{
    // ARRANGE
    const auto& input = R"""({
        function createMatrix(rows, cols, value) {
            var matrix = [];
            var i = 0;
            while( i < rows ) {
                var row = [];
                var j = 0;
                while( j < cols ) {
                    row[j] = value;
                    j = j + 1;
                }
                matrix[i] = row;
                i = i + 1;
            }
            return matrix;
        }
        
        function fillMatrix(matrix) {
            var i = 0;
            while( i < 2 ) {
                var j = 0;
                while( j < 2 ) {
                    matrix[i][j] = i * 2 + j + 1;
                    j = j + 1;
                }
                i = i + 1;
            }
            return matrix;
        }
        
        var mat = createMatrix(2, 2, 0);
        mat = fillMatrix(mat);
        return mat[1][1];
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_int(), true );
    EXPECT_EQ( result.get_int(), 4 ); // 1*2 + 1 + 1
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, SCOPE_AND_VARIABLE_SHADOWING )
{
    // ARRANGE
    const auto& input = R"""({
        var global_x = 100;
        var result = [];
        
        function outer(x) {
            var local_x = x + 10;
            
            function inner() {
                var x = local_x + 5;
                return x + global_x;
            }
            
            return inner();
        }
        
        result[0] = outer(1);  // (1+10+5) + 100 = 116
        result[1] = outer(2);  // (2+10+5) + 100 = 117
        
        return result;
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_array(), true );
    std::vector<Json> expected_array{ 116, 117 };
    EXPECT_EQ( result.get_array(), expected_array );
}

TEST( NAIVE_STACK_INTERPRETER_COMPLEX_SCENARIOS, CALCULATOR_WITH_OPERATIONS )
{
    // ARRANGE
    const auto& input = R"""({
        function add(a, b) { return a + b; }
        function multiply(a, b) { return a * b; }
        function subtract(a, b) { return a - b; }
        
        var operations = [add, multiply, subtract];
        var values = [10, 5];
        var results = [];
        
        var i = 0;
        while( i < 3 ) {
            results[i] = operations[i](values[0], values[1]);
            i = i + 1;
        }
        
        return results;
    })""";

    // ACT
    Interpreter interpreter;
    auto result = interpreter.eval( input );

    // ASSERT
    EXPECT_EQ( result.is_array(), true );
    std::vector<Json> expected_array{ 15, 50, 5 };  // 10+5, 10*5, 10-5
    EXPECT_EQ( result.get_array(), expected_array );
}
