#pragma once

#include <string>
#include "syntax_exception.hpp"
#include "json.h"

using namespace std;

class Interpreter
{
public:
   Json eval( const std::string& expression ) const;
};
