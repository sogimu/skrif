#include "EnvScope.h"
#include "json.h"

EnvScope::EnvScope()
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
}
EnvScope::EnvScope( const std::shared_ptr<EnvScope>& parent )
    : mParent{ parent }
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
}

EnvScope::~EnvScope()
{
    // std::cout << __PRETTY_FUNCTION__ << " "<< reinterpret_cast<uintptr_t>(this) <<")" << std::endl;
}

void EnvScope::set_parent( const std::shared_ptr<EnvScope>& parent )
{
  mParent = parent;
}

std::map< std::string, Json >::iterator EnvScope::begin(VaribleType varible_type)
{
    if( varible_type == VaribleType::Local )
    {
        return mVaribleByKey.begin();
    }
    return mVaribleByKey.begin();
}

std::map< std::string, Json >::iterator EnvScope::end(VaribleType varible_type)
{
    if( varible_type == VaribleType::Local )
    {
        return mVaribleByKey.end();
    }
    return mVaribleByKey.end();
}

void EnvScope::write(const std::string& key, const Json& value, VaribleType varible_type)
{
    auto current_scope = this->shared_from_this();
    auto value_copy = value;
    if( value_copy.is_function() )
    {
        auto& func = value_copy.get_function();
        if( func.getScope() == shared_from_this() )
        {
            func.makeWeakReference();
        }
        else 
        {
        
            func.makeStrongReference();
        }
    }
    if( varible_type == VaribleType::Global )
    {
        auto has_key = contains( key );
        if( has_key )
        {
            this->operator[](key) = std::move(value_copy);
        }
        else
        {
            current_scope->mVaribleByKey[ key ] = std::move(value_copy);
        }
    }
    else
    {
        current_scope->mVaribleByKey[ key ] = std::move(value_copy);
    }
}

Json& EnvScope::operator[](const std::string& key)
{
    auto current_scope = this->shared_from_this();
    bool done = false;
    while( current_scope != nullptr && !done )
    {
        if( auto it = current_scope->mVaribleByKey.find( key ); it != current_scope->mVaribleByKey.end() )
        {
            return it->second;
            done = true;
        }
        else
        {
            current_scope = current_scope->mParent;
        }
    }
    return mVaribleByKey[ key ];
}

const Json& EnvScope::operator[](const std::string& key) const
{
    auto current_scope = this->shared_from_this();
    bool done = false;
    while( current_scope != nullptr && !done )
    {
        if( auto it = current_scope->mVaribleByKey.find( key ); it != current_scope->mVaribleByKey.end() )
        {
            return it->second;
            done = true;
        }
        else
        {
            current_scope = current_scope->mParent;
        }
    }
    return mVaribleByKey[ key ];
}

bool EnvScope::contains(const std::string& key) const
{
    auto current_scope = this->shared_from_this();
    bool done = false;
    while( current_scope != nullptr && !done )
    {
        if( auto it = current_scope->mVaribleByKey.find( key ); it != current_scope->mVaribleByKey.end() )
        {
            return true;
            done = true;
        }
        else
        {
            current_scope = current_scope->mParent;
        }
    }
    return false;
}

std::shared_ptr<EnvScope> EnvScope::getParent() const
{
    return mParent;
}

void EnvScope::print() const
{
    std::cout << "  EnvScope " << reinterpret_cast<uintptr_t>(this) << ", " << reinterpret_cast<uintptr_t>(this->mParent.get()) << " contents:" << std::endl;
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
                std::cout << "Function object " << pair.second.get_function().get_text() << "env: " << reinterpret_cast<uintptr_t>(pair.second.get_function().getScope().get()) << (pair.second.get_function().isStrongReference() ? " with strong ref" :  " with weak ref");
            }
            else
            {
                std::cout << "Json object (" << pair.second << ")";
            }
            std::cout << std::endl;
        }
    }
}
