#include "interpreter.h"
#include <gtest/gtest.h>

TEST(BYTECODE_VM_IF, TRUE_BRANCH)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 0; if(1 == 1) { a = 1; } return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 1);
}

TEST(BYTECODE_VM_IF, FALSE_BRANCH_SKIPPED)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 0; if(1 == 2) { a = 1; } return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 0);
}

TEST(BYTECODE_VM_IF, ELSE_BRANCH)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 0; if(1 == 2) { a = 1; } else { a = 2; } return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 2);
}

TEST(BYTECODE_VM_IF, NESTED_IF)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 0; if(1 == 1) { if(2 == 2) { a = 42; } } return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_IF, CONDITION_WITH_VARIABLE)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 5; if(a > 3) { a = 100; } return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 100);
}

TEST(BYTECODE_VM_IF, VARIBLE_LESS_INT)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = 231; if( a < 1000 ) { return 234; } }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 234);
}

TEST(BYTECODE_VM_IF, INT_LESS_INT)
{
    Interpreter interp;
    auto result = interp.eval("{ if( 1 < 1000 ) { return 234; } }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 234);
}

TEST(BYTECODE_VM_IF, BIN_EXPR_LESS_BIN_EXPR)
{
    Interpreter interp;
    auto result = interp.eval("{ if( 1+2 < 1000+2000 ) { return 234; } }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 234);
}

TEST(BYTECODE_VM_IF, IF_ELSE_TRUE_BRANCH)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = 5; if( a > 3 ) { return 100; } else { return 200; } }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 100);
}

TEST(BYTECODE_VM_IF, IF_ELSE_FALSE_BRANCH)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = 1; if( a > 3 ) { return 100; } else { return 200; } }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 200);
}

TEST(BYTECODE_VM_IF, NESTED_IF_ELSE)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var a = 5;
        var b = 10;
        if( a > 3 ) {
            if( b > 8 ) { return 1; } else { return 2; }
        } else { return 3; }
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 1);
}

TEST(BYTECODE_VM_IF, IF_ELSE_WITH_VARIABLES)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var result = 0;
        var condition = 7;
        if( condition > 5 ) { result = condition * 2; } else { result = condition + 10; }
        return result;
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 14);
}

TEST(BYTECODE_VM_IF, IF_ELSE_CHAIN)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var score = 85;
        if( score >= 90 ) { return "A"; }
        else { if( score >= 80 ) { return "B"; } else { if( score >= 70 ) { return "C"; } else { return "F"; } } }
    })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "B");
}

TEST(BYTECODE_VM_IF, IF_WITH_ARRAY_CONDITION)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var arr = [1, 2, 3];
        if( arr[1] > 1 ) { return arr[2]; } else { return arr[0]; }
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 3);
}
