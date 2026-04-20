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
    for (int i = 0; i < static_cast<int>(int_constants.size()); ++i)
        if (int_constants[i] == v) return i;
    int_constants.push_back(v);
    return static_cast<int>(int_constants.size()) - 1;
}

int Chunk::add_double(double v)
{
    for (int i = 0; i < static_cast<int>(double_constants.size()); ++i)
        if (double_constants[i] == v) return i;
    double_constants.push_back(v);
    return static_cast<int>(double_constants.size()) - 1;
}

int Chunk::add_string(const std::string& s)
{
    for (int i = 0; i < static_cast<int>(string_constants.size()); ++i)
        if (string_constants[i] == s) return i;
    string_constants.push_back(s);
    return static_cast<int>(string_constants.size()) - 1;
}

int Chunk::add_function_proto(std::shared_ptr<json::Function> fp)
{
    function_protos.push_back(std::move(fp));
    return static_cast<int>(function_protos.size()) - 1;
}
