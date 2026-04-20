#pragma once
#include "json.h"
#include "call_frame.h"
#include <vector>
#include <memory>

class EnvScope;

class VM {
public:
    Json run(const Chunk& top_chunk, std::shared_ptr<EnvScope> global_env);

private:
    std::vector<Json>      stack_;
    std::vector<CallFrame> frames_;

    void        push(Json v);
    Json        pop();
    Json&       peek();
};
