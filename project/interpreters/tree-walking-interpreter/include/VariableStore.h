#pragma once

#include "EnvScope.h"
#include "json.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <deque>

class VariableStore1
{
public:
    // Конструктор создает корневой scope
    VariableStore1();
    
    // Деструктор
    ~VariableStore1() = default;
    
    // Создает новый scope связанный с текущим и делает его текущим
    std::shared_ptr<EnvScope> pushScope();
    void pushScope( const std::shared_ptr<EnvScope>& env ); 
    // Заменяет текущий scope на его parent
    void popScope();
    
    // Вызывает read у текущего scope
    // std::optional<Json*> operator[](const std::string& key);
    
    Json& operator[](const std::string& key);
    const Json& operator[](const std::string& key) const;
    
    // Вызывает write у текущего scope
    void write(const std::string& key, const Json& value, EnvScope::VaribleType varible_type = EnvScope::VaribleType::Global);
    
    void writeValueToLocalVarible( const std::string& name, const Json& value );
    
    // Получить текущий scope
    std::shared_ptr<EnvScope> getCurrentScope() const;
    
    // Распечатать содержимое всех scope-ов в иерархии
    void print() const;

private:
    // std::shared_ptr<EnvScope> mCurrentEnvScope;
    
  std::deque< std::shared_ptr<EnvScope> > mEnvScopeStack;
    // Для обработки случая когда operator[] не находит переменную
    // Json mNullJson;
};
