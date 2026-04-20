#pragma once
#include "opcode.h"
#include <cstdint>

struct Instruction {
    Opcode  opcode;
    int32_t operand = 0;
};
