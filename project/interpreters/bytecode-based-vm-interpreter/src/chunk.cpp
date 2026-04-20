#include "chunk.h"
#include "Function.h"

int Chunk::emit(Opcode op, int32_t operand)
{
    int idx = static_cast<int>(code.size());
    code.push_back({ op, operand });
    return idx;
}

void Chunk::patch_operand(int idx, int32_t val)
{
    code[idx].operand = val;
}

int Chunk::add_int(int v)
{
    auto [it, inserted] = int_index_.emplace(v, (int)int_constants.size());
    if (!inserted) return it->second;
    int_constants.push_back(v);
    return it->second;
}

int Chunk::add_double(double v)
{
    auto [it, inserted] = double_index_.emplace(v, (int)double_constants.size());
    if (!inserted) return it->second;
    double_constants.push_back(v);
    return it->second;
}

int Chunk::add_string(const std::string& s)
{
    auto [it, inserted] = string_index_.emplace(s, (int)string_constants.size());
    if (!inserted) return it->second;
    string_constants.push_back(s);
    return it->second;
}

int Chunk::add_function_proto(std::shared_ptr<json::Function> fp)
{
    function_protos.push_back(std::move(fp));
    return static_cast<int>(function_protos.size()) - 1;
}
