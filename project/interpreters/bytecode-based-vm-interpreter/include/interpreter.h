#pragma once
#include "json.h"
#include <string>

class Interpreter {
public:
    Json eval(const std::string& expression) const;
};
