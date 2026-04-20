#include "EnvScope.h"
#include "json.h"

EnvScope::EnvScope()
{
}
EnvScope::EnvScope( const std::shared_ptr<EnvScope>& parent )
    : mParent{ parent }
{
}

EnvScope::~EnvScope()
{
}

void EnvScope::set_parent( const std::shared_ptr<EnvScope>& parent )
{
    mParent = parent;
}

std::unordered_map< std::string, Json >::iterator EnvScope::begin(VaribleType)
{
    return mVaribleByKey.begin();
}

std::unordered_map< std::string, Json >::iterator EnvScope::end(VaribleType)
{
    return mVaribleByKey.end();
}

void EnvScope::write(const std::string& key, const Json& value, VaribleType varible_type)
{
    auto current_scope = this->shared_from_this();
    if( varible_type == VaribleType::Global )
    {
        auto has_key = contains( key );
        if( has_key )
        {
            this->operator[](key) = value;
        }
        else
        {
            current_scope->mVaribleByKey[ key ] = value;
        }
    }
    else
    {
        current_scope->mVaribleByKey[ key ] = value;
    }
}

Json& EnvScope::operator[](const std::string& key)
{
    auto current_scope = this->shared_from_this();
    while( current_scope != nullptr )
    {
        if( auto it = current_scope->mVaribleByKey.find( key ); it != current_scope->mVaribleByKey.end() )
        {
            return it->second;
        }
        current_scope = current_scope->mParent;
    }
    return mVaribleByKey[ key ];
}

const Json& EnvScope::operator[](const std::string& key) const
{
    auto current_scope = this->shared_from_this();
    while( current_scope != nullptr )
    {
        if( auto it = current_scope->mVaribleByKey.find( key ); it != current_scope->mVaribleByKey.end() )
        {
            return it->second;
        }
        current_scope = current_scope->mParent;
    }
    return mVaribleByKey[ key ];
}

bool EnvScope::contains(const std::string& key) const
{
    auto current_scope = this->shared_from_this();
    while( current_scope != nullptr )
    {
        if( auto it = current_scope->mVaribleByKey.find( key ); it != current_scope->mVaribleByKey.end() )
        {
            return true;
        }
        current_scope = current_scope->mParent;
    }
    return false;
}

std::shared_ptr<EnvScope> EnvScope::getParent() const
{
    return mParent;
}

void EnvScope::print() const
{
    std::cout << "  EnvScope " << reinterpret_cast<uintptr_t>(this) << " contents:" << std::endl;
    if (mVaribleByKey.empty())
    {
        std::cout << "    (empty)" << std::endl;
    }
    else
    {
        for (const auto& pair : mVaribleByKey)
        {
            std::cout << "    " << pair.first << " = ";
            if( pair.second.is_function() )
            {
                std::cout << "Function object " << pair.second.get_function().get_text();
            }
            else
            {
                std::cout << "Json object (" << pair.second << ")";
            }
            std::cout << std::endl;
        }
    }
}
