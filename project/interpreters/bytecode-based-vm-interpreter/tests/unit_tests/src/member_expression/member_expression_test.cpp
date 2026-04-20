#include "interpreter.h"
#include <gtest/gtest.h>

TEST(BYTECODE_VM_MEMBER, ARRAY_READ)
{
    Interpreter interp;
    auto result = interp.eval("{ arr = [1, 2, 3]; return arr[0]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 1);
}

TEST(BYTECODE_VM_MEMBER, ARRAY_WRITE)
{
    Interpreter interp;
    auto result = interp.eval("{ arr = [1, 2, 3]; arr[1] = 99; return arr[1]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 99);
}

TEST(BYTECODE_VM_MEMBER, OBJECT_READ)
{
    Interpreter interp;
    auto result = interp.eval(R"({ obj = {"x": 5}; return obj["x"]; })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 5);
}

TEST(BYTECODE_VM_MEMBER, OBJECT_WRITE)
{
    Interpreter interp;
    auto result = interp.eval(R"({ obj = {"x": 5}; obj["x"] = 42; return obj["x"]; })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_MEMBER, ARRAY_MUTATION_VISIBLE)
{
    // After arr[0]=100, loading arr again should show the mutation (shared_ptr semantics)
    Interpreter interp;
    auto result = interp.eval("{ arr = [1, 2, 3]; arr[0] = 100; return arr[0]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 100);
}

TEST(BYTECODE_VM_MEMBER, ARRAY_READ_INDEX)
{
    Interpreter interp;
    auto result = interp.eval("{ var arr = [10, 20, 30]; return arr[1]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 20);
}

TEST(BYTECODE_VM_MEMBER, EMPTY_ARRAY_READ_INDEX)
{
    Interpreter interp;
    auto result = interp.eval("{ var arr = []; return arr[1]; }");
    EXPECT_TRUE(result.is_null());
}

TEST(BYTECODE_VM_MEMBER, EMPTY_ARRAY_WRITE_INDEX)
{
    Interpreter interp;
    auto result = interp.eval("{ var arr = []; arr[1] = 42; return arr[1]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_MEMBER, ARRAY_MODIFY_AND_RETURN_ARRAY)
{
    Interpreter interp;
    auto result = interp.eval("{ var arr = [1, 2, 3]; arr[0] = 100; arr[2] = 300; return arr; }");
    EXPECT_TRUE(result.is_array());
    std::vector<Json> expected{100, 2, 300};
    EXPECT_EQ(result.get_array(), expected);
}

TEST(BYTECODE_VM_MEMBER, OBJECT_READ_PROPERTY)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"name": "John", "age": 30}; return obj["name"]; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "John");
}

TEST(BYTECODE_VM_MEMBER, OBJECT_WRITE_PROPERTY)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"name": "John", "age": 30}; obj["age"] = 35; return obj["age"]; })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 35);
}

TEST(BYTECODE_VM_MEMBER, OBJECT_ADD_NEW_PROPERTY)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"name": "John"}; obj["city"] = "Moscow"; return obj["city"]; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "Moscow");
}

TEST(BYTECODE_VM_MEMBER, OBJECT_MODIFY_AND_RETURN_OBJECT)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"a": 1, "b": 2}; obj["a"] = 10; obj["c"] = 3; return obj; })");
    EXPECT_TRUE(result.is_object());
    std::map<std::string, Json> expected{{"a", 10}, {"b", 2}, {"c", 3}};
    EXPECT_EQ(result.get_object(), expected);
}

TEST(BYTECODE_VM_MEMBER, NESTED_ARRAY_ACCESS)
{
    Interpreter interp;
    auto result = interp.eval("{ var nested = [[1, 2], [3, 4]]; return nested[1][0]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 3);
}

TEST(BYTECODE_VM_MEMBER, NESTED_OBJECT_ACCESS)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var nested = {"user": {"name": "Alice", "info": {"age": 25}}}; return nested["user"]["info"]["age"]; })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 25);
}

TEST(BYTECODE_VM_MEMBER, VARIABLE_INDEX_ACCESS)
{
    Interpreter interp;
    auto result = interp.eval("{ var arr = [10, 20, 30]; var index = 2; return arr[index]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 30);
}

TEST(BYTECODE_VM_MEMBER, EXPRESSION_INDEX_ACCESS)
{
    Interpreter interp;
    auto result = interp.eval("{ var arr = [100, 200, 300, 400]; return arr[1 + 1]; }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 300);
}

TEST(BYTECODE_VM_MEMBER, DOT_OBJECT_READ_PROPERTY)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"name": "Alice", "age": 30}; return obj.name; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "Alice");
}

TEST(BYTECODE_VM_MEMBER, DOT_OBJECT_WRITE_PROPERTY)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"name": "Alice", "age": 30}; obj.age = 25; return obj.age; })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 25);
}

TEST(BYTECODE_VM_MEMBER, DOT_OBJECT_ADD_PROPERTY)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var obj = {"name": "Alice"}; obj.city = "Moscow"; return obj.city; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "Moscow");
}

TEST(BYTECODE_VM_MEMBER, DOT_CHAINED_ACCESS)
{
    Interpreter interp;
    auto result = interp.eval(R"({ var nested = {"user": {"name": "Bob"}}; return nested["user"].name; })");
    EXPECT_TRUE(result.is_string());
    EXPECT_EQ(result.get_string(), "Bob");
}
