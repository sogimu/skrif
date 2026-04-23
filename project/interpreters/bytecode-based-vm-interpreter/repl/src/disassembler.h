#pragma once
#include "chunk.h"
#include <ostream>
#include <string>

void disassemble(std::ostream& out, const Chunk& chunk, const std::string& name);
