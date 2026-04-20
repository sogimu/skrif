#include "interpreter.h"
#include <gtest/gtest.h>

TEST(BYTECODE_VM_COMPLEX, FIBONACCI_RECURSIVE)
{
    Interpreter interp;
    auto result = interp.eval(R"(
    {
        function fib(n) {
            if(n == 0) { return 0; }
            if(n == 1) { return 1; }
            return fib(n - 1) + fib(n - 2);
        }
        return fib(10);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 55);
}

TEST(BYTECODE_VM_COMPLEX, FIBONACCI_ITERATIVE)
{
    Interpreter interp;
    auto result = interp.eval(R"(
    {
        function fib(n) {
            if(n == 0) { return 0; }
            if(n == 1) { return 1; }
            a = 0;
            b = 1;
            i = 2;
            while(i <= n) {
                c = a + b;
                a = b;
                b = c;
                i = i + 1;
            }
            return b;
        }
        return fib(10);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 55);
}

TEST(BYTECODE_VM_COMPLEX, CLOSURE_COUNTER)
{
    Interpreter interp;
    auto result = interp.eval(R"(
    {
        function make_counter() {
            count = 0;
            function inc() {
                count = count + 1;
                return count;
            }
            return inc;
        }
        counter = make_counter();
        counter();
        counter();
        return counter();
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 3);
}

TEST(BYTECODE_VM_COMPLEX, ARRAY_FILL_IN_LOOP)
{
    Interpreter interp;
    auto result = interp.eval(R"(
    {
        arr = [0, 0, 0, 0, 0];
        i = 0;
        while(i < 5) {
            arr[i] = i * 2;
            i = i + 1;
        }
        return arr[4];
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 8);
}

TEST(BYTECODE_VM_COMPLEX, WHILE_WITH_IF_AND_FUNCTIONS)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function isEven(n) { return (n / 2) * 2 == n; }
        var sum = 0;
        var i = 1;
        while( i <= 10 ) {
            if( isEven(i) ) { sum = sum + i; }
            i = i + 1;
        }
        return sum;
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 30);
}

TEST(BYTECODE_VM_COMPLEX, ARRAY_PROCESSING_WITH_FUNCTIONS)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function processArray(arr) {
            var result = [];
            var i = 0;
            while( i < 3 ) {
                if( arr[i] > 5 ) { result[i] = arr[i] * 2; }
                else { result[i] = arr[i] + 10; }
                i = i + 1;
            }
            return result;
        }
        var input_array = [3, 7, 1];
        return processArray(input_array);
    })");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{13, 14, 11};
    EXPECT_EQ(result.get_array(), expected);
}

TEST(BYTECODE_VM_COMPLEX, OBJECT_MANIPULATION_WITH_LOOPS)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var person = {"name": "John", "age": 25, "score": 0};
        var i = 1;
        while( i <= 5 ) { person["score"] = person["score"] + i; i = i + 1; }
        return person["score"];
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 15);
}

TEST(BYTECODE_VM_COMPLEX, RECURSIVE_TREE_TRAVERSAL)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function sumTree(node) {
            if( node["value"] == 0 ) { return 0; }
            var sum = node["value"];
            if( node["left"]["value"] != 0 ) { sum = sum + sumTree(node["left"]); }
            if( node["right"]["value"] != 0 ) { sum = sum + sumTree(node["right"]); }
            return sum;
        }
        var tree = {
            "value": 10,
            "left": {"value": 5, "left": {"value": 0}, "right": {"value": 0}},
            "right": {"value": 15, "left": {"value": 0}, "right": {"value": 0}}
        };
        return sumTree(tree);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 30);
}

TEST(BYTECODE_VM_COMPLEX, MATRIX_OPERATIONS)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function createMatrix(rows, cols, value) {
            var matrix = [];
            var i = 0;
            while( i < rows ) {
                var row = [];
                var j = 0;
                while( j < cols ) { row[j] = value; j = j + 1; }
                matrix[i] = row;
                i = i + 1;
            }
            return matrix;
        }
        function fillMatrix(matrix) {
            var i = 0;
            while( i < 2 ) {
                var j = 0;
                while( j < 2 ) { matrix[i][j] = i * 2 + j + 1; j = j + 1; }
                i = i + 1;
            }
            return matrix;
        }
        var mat = createMatrix(2, 2, 0);
        mat = fillMatrix(mat);
        return mat[1][1];
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 4);
}

TEST(BYTECODE_VM_COMPLEX, SCOPE_AND_VARIABLE_SHADOWING)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var global_x = 100;
        var result = [];
        function outer(x) {
            var local_x = x + 10;
            function inner() { var x = local_x + 5; return x + global_x; }
            return inner();
        }
        result[0] = outer(1);
        result[1] = outer(2);
        return result;
    })");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{116, 117};
    EXPECT_EQ(result.get_array(), expected);
}

TEST(BYTECODE_VM_COMPLEX, CALCULATOR_WITH_OPERATIONS)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function add(a, b) { return a + b; }
        function multiply(a, b) { return a * b; }
        function subtract(a, b) { return a - b; }
        var operations = [add, multiply, subtract];
        var values = [10, 5];
        var results = [];
        var i = 0;
        while( i < 3 ) { results[i] = operations[i](values[0], values[1]); i = i + 1; }
        return results;
    })");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{15, 50, 5};
    EXPECT_EQ(result.get_array(), expected);
}
