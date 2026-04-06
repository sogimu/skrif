#include "VariableStore.h"
#include "json.h"
#include "Function.h"
#include "EnvScope.h"

#include <gtest/gtest.h>

using namespace std;

// ============================================================
// Тесты конструктора и начального состояния
// ============================================================

TEST( VARIABLE_STORE, CONSTRUCTOR_CREATES_NULL_CURRENT_SCOPE )
{
    // ARRANGE & ACT
    VariableStore1 store;

    // ASSERT — после конструктора без pushScope текущий scope == nullptr
    EXPECT_EQ( store.getCurrentScope(), nullptr );
}

// ============================================================
// Тесты pushScope / popScope
// ============================================================

TEST( VARIABLE_STORE, PUSH_SCOPE_RETURNS_NON_NULL )
{
    // ARRANGE
    VariableStore1 store;

    // ACT
    auto scope = store.pushScope();

    // ASSERT
    EXPECT_NE( scope, nullptr );
    EXPECT_EQ( store.getCurrentScope(), scope );
}

TEST( VARIABLE_STORE, PUSH_SCOPE_CREATES_CHILD_WITH_PARENT )
{
    // ARRANGE
    VariableStore1 store;
    auto parent_scope = store.pushScope();

    // ACT
    auto child_scope = store.pushScope();

    // ASSERT
    EXPECT_NE( child_scope, nullptr );
    EXPECT_EQ( child_scope->getParent(), parent_scope );
    EXPECT_EQ( store.getCurrentScope(), child_scope );
}

TEST( VARIABLE_STORE, POP_SCOPE_RESTORES_PREVIOUS )
{
    // ARRANGE
    VariableStore1 store;
    auto first_scope = store.pushScope();
    auto second_scope = store.pushScope();

    // ACT
    store.popScope();

    // ASSERT
    EXPECT_EQ( store.getCurrentScope(), first_scope );
}

TEST( VARIABLE_STORE, PUSH_EXISTING_SCOPE_SETS_IT_AS_CURRENT )
{
    // ARRANGE
    VariableStore1 store;
    auto root = store.pushScope();
    auto external_scope = std::make_shared<EnvScope>( root );

    // ACT
    store.pushScope( external_scope );

    // ASSERT
    EXPECT_EQ( store.getCurrentScope(), external_scope );
}

TEST( VARIABLE_STORE, POP_AFTER_PUSH_EXISTING_SCOPE )
{
    // ARRANGE
    VariableStore1 store;
    auto root = store.pushScope();
    auto external_scope = std::make_shared<EnvScope>( root );
    store.pushScope( external_scope );

    // ACT
    store.popScope();

    // ASSERT
    EXPECT_EQ( store.getCurrentScope(), root );
}

TEST( VARIABLE_STORE, MULTIPLE_PUSH_POP_RETURNS_TO_ORIGINAL )
{
    // ARRANGE
    VariableStore1 store;
    auto scope1 = store.pushScope();
    auto scope2 = store.pushScope();
    auto scope3 = store.pushScope();

    // ACT
    store.popScope();
    store.popScope();

    // ASSERT
    EXPECT_EQ( store.getCurrentScope(), scope1 );
}

// ============================================================
// Тесты write / operator[]
// ============================================================

TEST( VARIABLE_STORE, WRITE_AND_READ_INT )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();

    // ACT
    store.write( "x", Json( 42 ) );

    // ASSERT
    EXPECT_TRUE( store["x"].is_int() );
    EXPECT_EQ( store["x"].get_int(), 42 );
}

TEST( VARIABLE_STORE, WRITE_AND_READ_STRING )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();

    // ACT
    store.write( "name", Json( std::string("hello") ) );

    // ASSERT
    EXPECT_TRUE( store["name"].is_string() );
    EXPECT_EQ( store["name"].get_string(), "hello" );
}

TEST( VARIABLE_STORE, WRITE_AND_READ_DOUBLE )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();

    // ACT
    store.write( "pi", Json( 3.14 ) );

    // ASSERT
    EXPECT_TRUE( store["pi"].is_double() );
    EXPECT_DOUBLE_EQ( store["pi"].get_double(), 3.14 );
}

TEST( VARIABLE_STORE, WRITE_AND_READ_BOOL )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();

    // ACT
    store.write( "flag", Json( true ) );

    // ASSERT
    EXPECT_TRUE( store["flag"].is_bool() );
    EXPECT_EQ( store["flag"].get_bool(), true );
}

TEST( VARIABLE_STORE, WRITE_OVERWRITES_EXISTING_VALUE )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();
    store.write( "x", Json( 1 ) );

    // ACT
    store.write( "x", Json( 99 ) );

    // ASSERT
    EXPECT_EQ( store["x"].get_int(), 99 );
}

TEST( VARIABLE_STORE, CONST_OPERATOR_BRACKET )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();
    store.write( "val", Json( 10 ) );

    // ACT
    const VariableStore1& const_store = store;

    // ASSERT
    EXPECT_TRUE( const_store["val"].is_int() );
    EXPECT_EQ( const_store["val"].get_int(), 10 );
}

// ============================================================
// Тесты writeValueToLocalVarible
// ============================================================

TEST( VARIABLE_STORE, WRITE_LOCAL_VARIABLE_NOT_VISIBLE_IN_PARENT )
{
    // ARRANGE
    VariableStore1 store;
    auto parent = store.pushScope();
    auto child = store.pushScope();

    // ACT — записываем локальную переменную в child scope
    store.writeValueToLocalVarible( "local_var", Json( 100 ) );

    // Проверяем что в child scope переменная видна
    EXPECT_EQ( store["local_var"].get_int(), 100 );

    // Переходим в parent scope
    store.popScope();

    // ASSERT — в parent scope переменной нет (operator[] создаст Null)
    EXPECT_TRUE( store["local_var"].is_null() );
}

TEST( VARIABLE_STORE, WRITE_LOCAL_VARIABLE_DOES_NOT_SHADOW_PARENT_AFTER_POP )
{
    // ARRANGE
    VariableStore1 store;
    auto parent = store.pushScope();
    store.write( "x", Json( 1 ) );

    auto child = store.pushScope();

    // ACT — записываем Local переменную в child с тем же именем
    store.writeValueToLocalVarible( "x", Json( 200 ) );
    EXPECT_EQ( store["x"].get_int(), 200 );

    // Pop child
    store.popScope();

    // ASSERT — parent scope не затронут
    EXPECT_EQ( store["x"].get_int(), 1 );
}

// ============================================================
// Тесты видимости переменных в иерархии scope-ов
// ============================================================

TEST( VARIABLE_STORE, CHILD_SCOPE_SEES_PARENT_VARIABLES )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();
    store.write( "parent_var", Json( 55 ) );

    // ACT — создаем дочерний scope
    store.pushScope();

    // ASSERT — дочерний scope видит переменную родителя
    EXPECT_TRUE( store["parent_var"].is_int() );
    EXPECT_EQ( store["parent_var"].get_int(), 55 );
}

TEST( VARIABLE_STORE, GLOBAL_WRITE_IN_CHILD_UPDATES_PARENT )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();
    store.write( "shared_var", Json( 10 ) );

    store.pushScope();

    // ACT — Global write должен обновить переменную в parent
    store.write( "shared_var", Json( 20 ), EnvScope::VaribleType::Global );

    // Pop child
    store.popScope();

    // ASSERT
    EXPECT_EQ( store["shared_var"].get_int(), 20 );
}

// ============================================================
// Тесты Function с weak_ptr / shared_ptr на scope
// ============================================================

// TEST( VARIABLE_STORE, FUNCTION_IN_OWN_SCOPE_HAS_WEAK_REFERENCE )
// {
//     // ARRANGE
//     VariableStore1 store;
//     auto scope = store.pushScope();
//
//     // Создаем Function с указателем на текущий scope (scope == родной)
//     json::Function func( nullptr, scope, "test_func", nullptr );
//     EXPECT_TRUE( func.isStrongReference() ); // Изначально strong
//
//     // ACT — записываем Function в scope, который является его "родным"
//     store.write( "myFunc", Json( func ) );
//
//     // ASSERT — после записи в родной scope ссылка должна стать weak
//     const auto& stored_func = store["myFunc"].get_function();
//     EXPECT_FALSE( stored_func.isStrongReference() );
// }

// TEST( VARIABLE_STORE, FUNCTION_IN_FOREIGN_SCOPE_HAS_STRONG_REFERENCE )
// {
//     // ARRANGE
//     VariableStore1 store;
//     auto scope1 = store.pushScope();
//
//     // Создаем Function с указателем на scope1
//     json::Function func( scope1, "test_func", nullptr );
//
//     // ACT — записываем Function в другой (child) scope
//     auto scope2 = store.pushScope();
//     store.write( "myFunc", Json( func ) );
//
//     // ASSERT — в чужом scope ссылка должна быть strong
//     const auto& stored_func = store["myFunc"].get_function();
//     EXPECT_TRUE( stored_func.isStrongReference() );
// }

// TEST( VARIABLE_STORE, FUNCTION_WEAK_REF_CAN_GET_SCOPE_WHILE_ALIVE )
// {
//     // ARRANGE
//     VariableStore1 store;
//     auto scope = store.pushScope();
//
//     json::Function func( scope, "func_alive", nullptr );
//     store.write( "fn", Json( func ) );
//
//     // ACT
//     const auto& stored_func = store["fn"].get_function();
//     auto retrieved_scope = stored_func.getScope();
//
//     // ASSERT — weak ссылка, но scope жив, поэтому getScope() != nullptr
//     EXPECT_FALSE( stored_func.isStrongReference() );
//     EXPECT_NE( retrieved_scope, nullptr );
//     EXPECT_EQ( retrieved_scope, scope );
// }
//
// TEST( VARIABLE_STORE, FUNCTION_STRONG_REF_KEEPS_SCOPE_ALIVE )
// {
//     // ARRANGE
//     json::Function func_copy( nullptr );
//
//     {
//         VariableStore1 store;
//         auto root = store.pushScope();
//
//         // Создаём функцию привязанную к root
//         json::Function func( root, "persisting_func", nullptr );
//
//         // Записываем в child scope — должен стать strong
//         auto child = store.pushScope();
//         store.write( "fn", Json( func ) );
//
//         // Копируем функцию
//         func_copy = store["fn"].get_function();
//         EXPECT_TRUE( func_copy.isStrongReference() );
//     }
//
//     // ACT — store уничтожен, но strong ref держит scope
//     auto scope_from_func = func_copy.getScope();
//
//     // ASSERT
//     EXPECT_NE( scope_from_func, nullptr );
// }
//
// TEST( VARIABLE_STORE, FUNCTION_MAKE_WEAK_THEN_STRONG )
// {
//     // ARRANGE
//     VariableStore1 store;
//     auto scope = store.pushScope();
//     json::Function func( scope, "toggle_func", nullptr );
//
//     // ACT — записываем в родной scope → weak
//     store.write( "fn", Json( func ) );
//     auto& stored_func = store["fn"].get_function();
//     EXPECT_FALSE( stored_func.isStrongReference() );
//
//     // Преобразуем обратно в strong
//     stored_func.makeStrongReference();
//     EXPECT_TRUE( stored_func.isStrongReference() );
//
//     // Преобразуем в weak
//     stored_func.makeWeakReference();
//     EXPECT_FALSE( stored_func.isStrongReference() );
// }
//
// TEST( VARIABLE_STORE, FUNCTION_IN_OWN_SCOPE_NO_CIRCULAR_REF_LEAK )
// {
//     // ARRANGE
//     std::weak_ptr<EnvScope> weak_scope;
//
//     {
//         VariableStore1 store;
//         auto scope = store.pushScope();
//         weak_scope = scope;
//
//         // Создаём функцию и записываем в родной scope
//         json::Function func( scope, "leak_test_func", nullptr );
//         store.write( "fn", Json( func ) );
//
//         // Ссылка должна быть weak — нет циклической зависимости
//         const auto& stored_func = store["fn"].get_function();
//         EXPECT_FALSE( stored_func.isStrongReference() );
//     }
//
//     // ASSERT — scope должен быть уничтожен (нет утечки)
//     EXPECT_TRUE( weak_scope.expired() );
// }
//
// TEST( VARIABLE_STORE, FUNCTION_MOVED_BETWEEN_SCOPES )
// {
//     // ARRANGE
//     VariableStore1 store;
//     auto scope1 = store.pushScope();
//
//     // Создаём функцию с scope1 как родным
//     json::Function func( scope1, "movable_func", nullptr );
//
//     // Записываем в scope1 (родной) → weak
//     store.write( "fn", Json( func ) );
//     EXPECT_FALSE( store["fn"].get_function().isStrongReference() );
//
//     // ACT — создаём scope2, читаем функцию из scope1 (видна через parent) и пишем в scope2
//     auto scope2 = store.pushScope();
//     auto fn_json = store["fn"]; // читаем из parent chain
//     store.write( "fn_copy", fn_json ); // пишем в scope2 (не родной для функции)
//
//     // ASSERT — в scope2 функция должна иметь strong reference (scope2 != scope1)
//     const auto& copied_func = store["fn_copy"].get_function();
//     EXPECT_TRUE( copied_func.isStrongReference() );
// }
//
TEST( VARIABLE_STORE, FUNCTION_WITH_NULL_SCOPE )
{
    // ARRANGE
    VariableStore1 store;
    auto scope = store.pushScope();

    json::Function func( nullptr, "null_scope_func", nullptr );

    // ACT — записываем функцию с nullptr scope
    store.write( "fn", Json( func ) );

    // ASSERT — getScope() для функции с nullptr == nullptr и !isStrongReference
    const auto& stored_func = store["fn"].get_function();
    // nullptr scope != scope, поэтому makeStrongReference будет вызван
    // Но weak_ptr от nullptr всё равно expired, так что strong от nullptr остаётся nullptr
    auto retrieved = stored_func.getScope();
    EXPECT_EQ( retrieved, nullptr );
}

// ============================================================
// Тесты getCurrentScope
// ============================================================

TEST( VARIABLE_STORE, GET_CURRENT_SCOPE_AFTER_PUSH )
{
    // ARRANGE
    VariableStore1 store;

    // ACT
    auto scope = store.pushScope();

    // ASSERT
    EXPECT_EQ( store.getCurrentScope(), scope );
}

TEST( VARIABLE_STORE, GET_CURRENT_SCOPE_AFTER_POP )
{
    // ARRANGE
    VariableStore1 store;
    auto scope1 = store.pushScope();
    store.pushScope();

    // ACT
    store.popScope();

    // ASSERT
    EXPECT_EQ( store.getCurrentScope(), scope1 );
}

// ============================================================
// Тесты с массивами и объектами
// ============================================================

TEST( VARIABLE_STORE, WRITE_AND_READ_ARRAY )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();

    std::vector<Json> arr{ Json(1), Json(2), Json(3) };

    // ACT
    store.write( "arr", Json( arr ) );

    // ASSERT
    EXPECT_TRUE( store["arr"].is_array() );
    EXPECT_EQ( store["arr"].get_array().size(), 3u );
    EXPECT_EQ( store["arr"].get_array()[0].get_int(), 1 );
    EXPECT_EQ( store["arr"].get_array()[2].get_int(), 3 );
}

TEST( VARIABLE_STORE, WRITE_AND_READ_OBJECT )
{
    // ARRANGE
    VariableStore1 store;
    store.pushScope();

    std::map<std::string, Json> obj{ {"key1", Json(10)}, {"key2", Json("val")} };

    // ACT
    store.write( "obj", Json( obj ) );

    // ASSERT
    EXPECT_TRUE( store["obj"].is_object() );
    EXPECT_EQ( store["obj"]["key1"].get_int(), 10 );
    EXPECT_EQ( store["obj"]["key2"].get_string(), "val" );
}
