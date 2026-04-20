#include "Function.h"
#include "EnvScope.h"

using namespace json;

Function::Function(std::shared_ptr<EnvScope> closure,
                   std::string name,
                   std::vector<std::string> params,
                   std::shared_ptr<Chunk> body)
    : mName(std::move(name))
    , mParams(std::move(params))
    , mBody(std::move(body))
    , mClosure(std::move(closure))
{
}

std::shared_ptr<EnvScope> Function::getScope() const
{
    return mClosure;
}

void Function::clearScope()
{
    mClosure = nullptr;
}
