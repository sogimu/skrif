#include "interpreter.h"
#include <gtest/gtest.h>

TEST(BYTECODE_VM_VARIABLE, GLOBAL_ASSIGNMENT_AND_READ)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 5; return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 5);
}

TEST(BYTECODE_VM_VARIABLE, LOCAL_ASSIGNMENT_AND_READ)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = 10; return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 10);
}

TEST(BYTECODE_VM_VARIABLE, REASSIGNMENT)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 5; a = 10; return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 10);
}

TEST(BYTECODE_VM_VARIABLE, EXPRESSION_WITH_VARIABLE)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 5; return a + 3; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 8);
}

TEST(BYTECODE_VM_VARIABLE, TWO_VARIABLES)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 3; b = 7; return a + b; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 10);
}

TEST(BYTECODE_VM_VARIABLE, STRING_ASSIGNMENT)
{
    Interpreter interp;
    auto result = interp.eval(R"({ a = "hello"; return a; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "hello");
}

TEST(BYTECODE_VM_VARIABLE, ARRAY_ASSIGNMENT)
{
    Interpreter interp;
    auto result = interp.eval("{ a = [1, 2, 3]; return a; }");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{1, 2, 3};
    EXPECT_EQ(result.get_array(), expected);
}

TEST(BYTECODE_VM_VARIABLE, OBJECT_ASSIGNMENT)
{
    Interpreter interp;
    auto result = interp.eval(R"({ a = {"x": 1, "y": 2}; return a; })");
    EXPECT_TRUE(result.is_object());
    std::map<std::string, Json> expected{{"x", 1}, {"y", 2}};
    EXPECT_EQ(result.get_object(), expected);
}

TEST(BYTECODE_VM_VARIABLE, LOCAL_READ_WRITE_INT)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = 1; return a; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 1);
}

TEST(BYTECODE_VM_VARIABLE, LOCAL_READ_WRITE_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = 1.5; return a; }");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 1.5);
}

TEST(BYTECODE_VM_VARIABLE, LOCAL_READ_WRITE_STRING)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var a = "some string"; return a; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "some string");
}

TEST(BYTECODE_VM_VARIABLE, LOCAL_READ_WRITE_ARRAY)
{
    Interpreter interp;
    auto result = interp.eval("{ var a = [1,2,3]; return a; }");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{1, 2, 3};
    EXPECT_EQ(result.get_array(), expected);
}

TEST(BYTECODE_VM_VARIABLE, LOCAL_READ_WRITE_OBJECT)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var a = {"a": 1, "b": 2}; return a; })");
    EXPECT_TRUE(result.is_object());
    std::map<std::string, Json> expected{{"a", 1}, {"b", 2}};
    EXPECT_EQ(result.get_object(), expected);
}

TEST(BYTECODE_VM_VARIABLE, GLOBAL_READ_WRITE_DOUBLE)
{
    Interpreter interp;
    auto result = interp.eval("{ a = 1.5; return a; }");
    EXPECT_TRUE(result.is_double());
    EXPECT_DOUBLE_EQ(result.get_double(), 1.5);
}

TEST(BYTECODE_VM_VARIABLE, GLOBAL_READ_WRITE_STRING)
{
    Interpreter interp;
    auto result = interp.eval(R"({ a = "some string"; return a; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "some string");
}

TEST(BYTECODE_VM_VARIABLE, GLOBAL_READ_WRITE_ARRAY)
{
    Interpreter interp;
    auto result = interp.eval("{ a = [1,2,3]; return a; }");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{1, 2, 3};
    EXPECT_EQ(result.get_array(), expected);
}

TEST(BYTECODE_VM_VARIABLE, GLOBAL_READ_WRITE_OBJECT)
{
    Interpreter interp;
    auto result = interp.eval(R"({ a = {"a": 1, "b": 2}; return a; })");
    EXPECT_TRUE(result.is_object());
    std::map<std::string, Json> expected{{"a", 1}, {"b", 2}};
    EXPECT_EQ(result.get_object(), expected);
}
