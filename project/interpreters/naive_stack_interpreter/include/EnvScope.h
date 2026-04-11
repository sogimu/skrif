#pragma once

#include <memory>
#include <map>
#include <string>
#include <optional>
#include <iostream>

// Предварительное объявление класса Json
class Json;

class EnvScope : public std::enable_shared_from_this<EnvScope>
{
public:
    enum class VaribleType
    {
        Local,
        Global
    };

    EnvScope();
    EnvScope( const std::shared_ptr<EnvScope>& parent );
    ~EnvScope();
    void set_parent( const std::shared_ptr<EnvScope>& parent );

    void write(const std::string& key, const Json& value, VaribleType varible_type = VaribleType::Global);
    Json& operator[](const std::string& key);
    const Json& operator[](const std::string& key) const;
    bool contains(const std::string& key) const;
    
    std::map< std::string, Json >::iterator begin(VaribleType varible_type);
    std::map< std::string, Json >::iterator end(VaribleType varible_type);
    // Получить родительский scope
    std::shared_ptr<EnvScope> getParent() const;
    
    // Распечатать содержимое данного scope
    void print() const;

private:
    std::shared_ptr<EnvScope> mParent;
    mutable std::map< std::string, Json > mVaribleByKey;
};
