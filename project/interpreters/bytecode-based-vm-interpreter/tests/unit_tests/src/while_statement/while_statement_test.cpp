#include "interpreter.h"
#include <gtest/gtest.h>

TEST(BYTECODE_VM_WHILE, BASIC_LOOP)
{
    Interpreter interp;
    auto result = interp.eval("{ i = 0; while(i < 5) { i = i + 1; } return i; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 5);
}

TEST(BYTECODE_VM_WHILE, LOOP_WITH_ACCUMULATOR)
{
    Interpreter interp;
    auto result = interp.eval("{ sum = 0; i = 1; while(i <= 10) { sum = sum + i; i = i + 1; } return sum; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 55);
}

TEST(BYTECODE_VM_WHILE, ZERO_ITERATIONS)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 42; while(1 == 0) { a = 0; } return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_WHILE, SIMPLE_COUNTER_VAR)
{
    Interpreter interp;
    auto result = interp.eval("{ var counter = 0; while( counter < 5 ) { counter = counter + 1; } return counter; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 5);
}

TEST(BYTECODE_VM_WHILE, FACTORIAL)
{
    Interpreter interp;
    auto result = interp.eval("{ var n = 5; var fact = 1; var i = 1; while( i <= n ) { fact = fact * i; i = i + 1; } return fact; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 120);
}

TEST(BYTECODE_VM_WHILE, NESTED_LOOP)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var total = 0;
        var i = 1;
        while( i <= 3 ) {
            var j = 1;
            while( j <= 2 ) { total = total + 1; j = j + 1; }
            i = i + 1;
        }
        return total;
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 6);
}

TEST(BYTECODE_VM_WHILE, BREAK_CONDITION_FALSE)
{
    Interpreter interp;
    auto result = interp.eval("{ var i = 10; while( i < 5 ) { i = i + 1; } return i; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 10);
}

TEST(BYTECODE_VM_WHILE, WITH_ARRAY_MODIFICATION)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var arr = [1, 2, 3];
        var i = 0;
        while( i < 3 ) { arr[i] = arr[i] * 2; i = i + 1; }
        return arr;
    })");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{2, 4, 6};
    EXPECT_EQ(result.get_array(), expected);
}
