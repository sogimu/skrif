#include "VariableStore.h"
#include "EnvScope.h"
#include <iostream>
#include <optional>

VariableStore1::VariableStore1()
    // : mCurrentEnvScope(std::make_shared<EnvScope>())
{
    // std::cout << "VariableStore::VariableStore()" << std::endl;
}

std::shared_ptr<EnvScope> VariableStore1::pushScope()
{
    // Создаем новый scope с текущим как parent
  std::shared_ptr<EnvScope> parent = mEnvScopeStack.empty() ? nullptr : mEnvScopeStack.back();
    auto newEnvScope = std::make_shared<EnvScope>(parent);

    // std::cout << "VariableStore::pushEnvScope(" <<reinterpret_cast<uintptr_t>(newEnvScope.get()) <<")" << std::endl;
    
    
    // Делаем новый scope текущим
    mEnvScopeStack.push_back( newEnvScope );
    // print();
    return mEnvScopeStack.back();
}

void VariableStore1::pushScope( const std::shared_ptr<EnvScope>& env )
{
    // std::cout << "VariableStore::pushEnvScope( exsisted " <<reinterpret_cast<uintptr_t>(env.get()) <<")" << std::endl;
    // env->print();

    // Делаем новый scope текущим
    mEnvScopeStack.push_back( env );
    // print();
}
void VariableStore1::popScope()
{
    // std::cout << "VariableStore::popEnvScope() " <<reinterpret_cast<uintptr_t>(mEnvScopeStack.back().get()) << std::endl;
    //
    // print();
    // auto parent = mCurrentEnvScope->getParent();
    // if (parent)
    // {
    if( mEnvScopeStack.size() == 1 )
    {
        auto& last_scope = mEnvScopeStack.back();
        for ( auto it = last_scope->begin(EnvScope::VaribleType::Local) ; it != last_scope->end(EnvScope::VaribleType::Local); ++it)
        {
            auto& value = *it;
            if( value.second.is_function() )
            {
                auto& func = value.second.get_function();
                func.makeWeakReference();
            }
        }
    }
        mEnvScopeStack.pop_back();
    // }
    // else
    // {
    //     // Пытаемся pop корневой scope - это может быть ошибка или допустимо
    //     // Выберем вариант с предупреждением но без исключения
    //     std::cout << "Warning: Trying to pop root scope, ignoring" << std::endl;
    // }
}

// std::optional<Json*> VariableStore1::operator[](const std::string& key)
// {
//     std::cout << "VariableStore::operator[](" << key << ")" << std::endl;
//
//     return mCurrentEnvScope->read(key);
// }

Json& VariableStore1::operator[](const std::string& key)
{
    // std::cout << "VariableStore::operator[](" << key << ")" << std::endl;
   
    return mEnvScopeStack.back()->operator[](key);
}

const Json& VariableStore1::operator[](const std::string& key) const
{
    // std::cout << "VariableStore::operator[](" << key << ")" << std::endl;
   
    return mEnvScopeStack.back()->operator[](key);
}

void VariableStore1::write(const std::string& key, const Json& value, EnvScope::VaribleType varible_type)
{
    // std::cout << "VariableStore::write(" << key << ") = " << value << std::endl;
    mEnvScopeStack.back()->write(key, value, varible_type);
}

void VariableStore1::writeValueToLocalVarible( const std::string& name, const Json& value )
{
  write(name, value, EnvScope::VaribleType::Local);
}

std::shared_ptr<EnvScope> VariableStore1::getCurrentScope() const
{
    if( mEnvScopeStack.empty() )
    {
      return {};
    }
    return mEnvScopeStack.back();
}

void VariableStore1::print() const
{
    std::cout << "VariableStore::print()" << std::endl;
    std::cout << "=== Variable Store Contents ===" << std::endl;
    
    // Печатаем scope-ы от корневого к текущему
    int level = 0;
    std::shared_ptr<EnvScope> current;
    for (auto it = mEnvScopeStack.begin(); it != mEnvScopeStack.end(); ++it, ++level)
    {
        std::cout << (current ? "| " : "* ") << "EnvScope level " << level << (level == 0 ? " (current):" : ":") ;
        (*it)->print();
        current = it->get()->getParent();
    }
    
    std::cout << "===============================" << std::endl;
}
