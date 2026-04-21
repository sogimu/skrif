#pragma once
#include <memory>
#include <vector>
#include "json.h"

class EnvScope;
struct Chunk;

struct CallFrame {
    const Chunk*              chunk;
    int                       ip;
    int                       base_stack_size;
    std::shared_ptr<EnvScope> env;
    std::vector<Json>         locals;
};
