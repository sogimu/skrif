#include "Function.h"
#include "EnvScope.h"
#include <ostream>

using namespace json;

// Function::~Function()
// {
//   std::cout << __PRETTY_FUNCTION__ << std::endl;  
// }

Function::Function(std::shared_ptr<EnvScope> outer_env_scope, const std::string& text, std::shared_ptr<ISyntaxNode> ast)
    : mText(text)
    , mAst(ast)
    , mOuter_env_scope(outer_env_scope)
{
    // std::cout << __PRETTY_FUNCTION__ << " "<< reinterpret_cast<uintptr_t>(this) << " env: "<< reinterpret_cast<uintptr_t>(outer_env_scope.get()) << std::endl;
}


Function::~Function()
{
    // std::cout << __PRETTY_FUNCTION__ << " "<< reinterpret_cast<uintptr_t>(this) << " env: "<< reinterpret_cast<uintptr_t>(getScope().get()) << std::endl;
//   if( !mClousures )
//     return;
//   mOuterEnvScopeIt.reset();
//   // mClousures->erase( mOuterEnvScopeIt );
//     // std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::shared_ptr<EnvScope> Function::getScope() const
{
    return mOuter_env_scope.lock();
}
