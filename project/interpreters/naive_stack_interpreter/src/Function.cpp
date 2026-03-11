#include "Function.h"
#include "EnvScope.h"

using namespace json;

Function::Function( const std::shared_ptr< EnvScope >& global_scope )
    : mEnvScope{ global_scope }
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Function::Function( const Function& function )
  : mEnvScope{ function.mEnvScope }
  , mAst{ function.mAst }
  , mText{ function.mText }
{
    makeStrongReference();
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Function::Function( Function&& function )
  : mEnvScope{ std::move(function.mEnvScope) }
  , mAst{ std::move(function.mAst) }
  , mText{ std::move(function.mText) }
{

    // std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Function& Function::operator=(const Function& function)
{

    // std::cout << __PRETTY_FUNCTION__ << std::endl;
  mEnvScope = function.mEnvScope;
  mAst = function.mAst;
  mText = function.mText;
    makeStrongReference();
  return *this;
}

Function& Function::operator=(Function&& function)
{

    // std::cout << __PRETTY_FUNCTION__ << std::endl;
  mEnvScope = std::move(function.mEnvScope);
  mAst = std::move(function.mAst);
  mText = std::move(function.mText);
  return *this;
}

Function::~Function()
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Function::makeStrongReference()
{
    if (std::holds_alternative< std::weak_ptr< EnvScope > >(mEnvScope))
    {
        auto weak_scope = std::get< std::weak_ptr< EnvScope > >(mEnvScope);
        auto shared_scope = weak_scope.lock();
        if (shared_scope)
        {
            mEnvScope = shared_scope;
            // std::cout << "Function: Converted to strong reference: " << mText << std::endl;
        }
        else
        {
            // std::cout << "Function: Warning - weak reference is expired, cannot convert to strong" << std::endl;
        }
    }
}

void Function::makeWeakReference()
{
    if (std::holds_alternative< std::shared_ptr< EnvScope > >(mEnvScope))
    {
        auto shared_scope = std::get< std::shared_ptr< EnvScope > >(mEnvScope);
        mEnvScope = std::weak_ptr< EnvScope >{ shared_scope };
        // std::cout << "Function: Converted to weak reference: " << mText << std::endl;
    }
}

bool Function::isStrongReference() const
{
    return std::holds_alternative< std::shared_ptr< EnvScope > >(mEnvScope);
}

std::shared_ptr< EnvScope > Function::getScope() const
{
    if (std::holds_alternative< std::shared_ptr< EnvScope > >(mEnvScope))
    {
        return std::get< std::shared_ptr< EnvScope > >(mEnvScope);
    }
    
    // const auto& ddd = std::get< std::shared_ptr< EnvScope > >(mEnvScope);
    // (void) ddd;
    auto weak_scope = std::get< std::weak_ptr< EnvScope > >(mEnvScope);
    auto ss = weak_scope.lock();
    if( !ss )
    {
        std::cout << "NON EXISTED ENV SCOPE!" << std::endl;
    } 
    return ss;
}
