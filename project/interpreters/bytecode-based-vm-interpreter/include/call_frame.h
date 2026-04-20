#pragma once
#include <memory>

class EnvScope;
struct Chunk;

struct CallFrame {
    const Chunk*              chunk;
    int                       ip;
    int                       base_stack_size;
    std::shared_ptr<EnvScope> env;
};
