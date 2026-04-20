#include "interpreter.h"
#include <gtest/gtest.h>

using namespace std;

TEST(BYTECODE_VM_ARITHMETIC, NUMBER_INT)
{
    Interpreter interp;
    auto result = interp.eval("1;");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 1);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_ADDITION_INT)
{
    Interpreter interp;
    auto result = interp.eval("1+2;");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 3);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_SUBSTRACTION_INT)
{
    Interpreter interp;
    auto result = interp.eval("5-3;");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 2);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_MULTIPLY_INT)
{
    Interpreter interp;
    auto result = interp.eval("2*3;");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 6);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_DIVIDE_INT)
{
    Interpreter interp;
    auto result = interp.eval("1/2;");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 0);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_ADDITION_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("1.5+2.3;");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 3.8);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_DIVIDE_INT)
{
    Interpreter interp;
    auto result = interp.eval("1.0/2;");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 0.5);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2==2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_NOT_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2!=3;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_LESS_INT)
{
    Interpreter interp;
    auto result = interp.eval("1<2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_LESS_OR_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2<=2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_MORE_INT)
{
    Interpreter interp;
    auto result = interp.eval("2>1;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_MORE_OR_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2>=2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, RETURN_INT)
{
    Interpreter interp;
    auto result = interp.eval("return 1;");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 1);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_DIVIDE_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("1/2.0;");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 0.5);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_MULTIPLY_INT)
{
    Interpreter interp;
    auto result = interp.eval("2.0*2;");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 4.0);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_MULTIPLY_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("2*2.0;");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 4.0);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2.0==2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_EQUAL_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("2==2.0;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_LESS_INT)
{
    Interpreter interp;
    auto result = interp.eval("1.0<2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_LESS_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("1<2.0;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_MORE_INT)
{
    Interpreter interp;
    auto result = interp.eval("2.0>1;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_MORE_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("2>1.0;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_LESS_OR_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2.0<=2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_LESS_OR_EQUAL_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("2<=2.0;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_DOUBLE_MORE_OR_EQUAL_INT)
{
    Interpreter interp;
    auto result = interp.eval("2.0>=2;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, BIN_EXPR_INT_MORE_OR_EQUAL_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("2>=2.0;");
    EXPECT_TRUE(result.is_bool());
    EXPECT_EQ(result.get_bool(), true);
}

TEST(BYTECODE_VM_ARITHMETIC, FUNCTION_CALL_INT)
{
    Interpreter interp;
    auto result = interp.eval("{ function foo(arg){ return arg+1; } return foo(1); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 2);
}
