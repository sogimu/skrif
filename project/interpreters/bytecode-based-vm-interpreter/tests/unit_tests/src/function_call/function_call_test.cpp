#include "interpreter.h"
#include <gtest/gtest.h>

TEST(BYTECODE_VM_FUNCTION, SIMPLE_RETURN)
{
    Interpreter interp;
    auto result = interp.eval("{ function foo(arg) { return arg + 1; } return foo(1); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 2);
}

TEST(BYTECODE_VM_FUNCTION, NO_ARGS)
{
    Interpreter interp;
    auto result = interp.eval("{ function answer() { return 42; } return answer(); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_FUNCTION, TWO_ARGS)
{
    Interpreter interp;
    auto result = interp.eval("{ function add(a, b) { return a + b; } return add(3, 4); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 7);
}

TEST(BYTECODE_VM_FUNCTION, CLOSURE_CAPTURE)
{
    Interpreter interp;
    auto result = interp.eval(R"(
    {
        x = 10;
        function get_x() { return x; }
        return get_x();
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 10);
}

TEST(BYTECODE_VM_FUNCTION, RECURSIVE)
{
    Interpreter interp;
    auto result = interp.eval(R"(
    {
        function fact(n) {
            if(n <= 1) { return 1; }
            return n * fact(n - 1);
        }
        return fact(5);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 120);
}

TEST(BYTECODE_VM_FUNCTION, RETURN_INT)
{
    Interpreter interp;
    auto result = interp.eval("{ function foo() { return 234; } return foo(); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 234);
}

TEST(BYTECODE_VM_FUNCTION, CALL_WITH_INT)
{
    Interpreter interp;
    auto result = interp.eval("{ function foo(val) { return 234 + val; } return foo(1); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 235);
}

TEST(BYTECODE_VM_FUNCTION, CALL_WITH_INT_TWO_TIMES)
{
    Interpreter interp;
    auto result = interp.eval("{ function foo(val) { return 234 + val; } return foo(1) + foo(2); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 471);
}

TEST(BYTECODE_VM_FUNCTION, RECURSIVE_CALL_WITH_COUNTER)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var counter = 0;
        function foo(val) {
            counter = counter + 1;
            if( val > 0 ) { foo(val-1); }
        }
        foo(5);
        return counter;
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 6);
}

TEST(BYTECODE_VM_FUNCTION, MULTIPLE_PARAMETERS)
{
    Interpreter interp;
    auto result = interp.eval("{ function add(a, b, c) { return a + b + c; } return add(10, 20, 30); }");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 60);
}

TEST(BYTECODE_VM_FUNCTION, MEMBER_NOTATION_BRACKET_NO_ARGS)
{
    Interpreter interp;
    auto result = interp.eval(R"({ function greet() { return 42; } var obj = {}; obj["fn"] = greet; return obj["fn"](); })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_FUNCTION, MEMBER_NOTATION_DOT_NO_ARGS)
{
    Interpreter interp;
    auto result = interp.eval(R"({ function greet() { return 42; } var obj = {}; obj.fn = greet; return obj.fn(); })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 42);
}

TEST(BYTECODE_VM_FUNCTION, MEMBER_NOTATION_BRACKET_WITH_ARGS)
{
    Interpreter interp;
    auto result = interp.eval(R"({ function add(a, b) { return a + b; } var obj = {}; obj["fn"] = add; return obj["fn"](10, 20); })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 30);
}

TEST(BYTECODE_VM_FUNCTION, MEMBER_NOTATION_DOT_WITH_ARGS)
{
    Interpreter interp;
    auto result = interp.eval(R"({ function add(a, b) { return a + b; } var obj = {}; obj.fn = add; return obj.fn(10, 20); })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 30);
}

TEST(BYTECODE_VM_FUNCTION, LOCAL_VARIABLE_SCOPE)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var global_var = 100;
        function test() { var local_var = 50; return local_var + global_var; }
        return test();
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 150);
}

TEST(BYTECODE_VM_FUNCTION, GLOBAL_VARIABLE_MODIFICATION)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        global_counter = 0;
        function increment() { global_counter = global_counter + 1; return global_counter; }
        increment();
        increment();
        return increment();
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 3);
}

TEST(BYTECODE_VM_FUNCTION, LOCAL_VARIABLE_SHADOW_GLOBAL)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var x = 100;
        function test() { var x = 50; return x; }
        return test();
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 50);
}

TEST(BYTECODE_VM_FUNCTION, NESTED_FUNCTIONS)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function outer(x) {
            function inner(y) { return x + y; }
            return inner(10);
        }
        return outer(5);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 15);
}

TEST(BYTECODE_VM_FUNCTION, FUNCTION_RETURNING_FUNCTION)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function createMultiplier(factor) {
            function multiply(value) { return value * factor; }
            return multiply;
        }
        var doubler = createMultiplier(2);
        return doubler(5);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 10);
}

TEST(BYTECODE_VM_FUNCTION, PARAMETER_SCOPE_ISOLATION)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        var global_val = 100;
        function test(global_val) { return global_val + 1; }
        return test(50);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 51);
}

TEST(BYTECODE_VM_FUNCTION, RECURSIVE_FIBONACCI)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function fib(n) {
            if( n <= 1 ) { return n; } else { return fib(n - 1) + fib(n - 2); }
        }
        return fib(7);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 13);
}

TEST(BYTECODE_VM_FUNCTION, FUNCTION_WITH_ARRAY_PARAMETER)
{
    Interpreter interp;
    auto result = interp.eval(R"({
        function sum_array(arr) {
            var total = 0; var i = 0;
            while( i < 3 ) { total = total + arr[i]; i = i + 1; }
            return total;
        }
        return sum_array([10, 20, 30]);
    })");
    EXPECT_TRUE(result.is_int());
    EXPECT_EQ(result.get_int(), 60);
}
