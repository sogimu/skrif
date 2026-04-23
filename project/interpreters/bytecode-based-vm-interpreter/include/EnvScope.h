#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include <iostream>

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

    std::unordered_map< std::string, Json >::iterator begin(VaribleType varible_type);
    std::unordered_map< std::string, Json >::iterator end(VaribleType varible_type);
    std::shared_ptr<EnvScope> getParent() const;

    void print() const;

public:
    bool has_closures = false;

private:
    std::shared_ptr<EnvScope> mParent;
    mutable std::unordered_map< std::string, Json > mVaribleByKey;
};
