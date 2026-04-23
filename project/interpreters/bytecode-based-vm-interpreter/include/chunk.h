#pragma once
#include "instruction.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace json { class Function; }

struct Chunk {
    std::vector<Instruction>                     code;
    std::vector<int>                             int_constants;
    std::vector<double>                          double_constants;
    std::vector<std::string>                     string_constants;
    std::vector<std::shared_ptr<json::Function>> function_protos;
    int                                          num_locals = 0;

private:
    std::unordered_map<int, int>         int_index_;
    std::unordered_map<double, int>      double_index_;
    std::unordered_map<std::string, int> string_index_;

public:

    int  emit(Opcode op, int32_t operand = 0);
    void patch_operand(int idx, int32_t val);
    int  add_int(int v);
    int  add_double(double v);
    int  add_string(const std::string& s);
    int  add_function_proto(std::shared_ptr<json::Function> fp);
};
