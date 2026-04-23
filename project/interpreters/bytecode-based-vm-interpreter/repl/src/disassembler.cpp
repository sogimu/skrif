#include "disassembler.h"
#include "Function.h"

#include <iomanip>
#include <sstream>
#include <vector>

static std::string quoted(const std::string& s)
{
    return "\"" + s + "\"";
}

static std::string params_str(const std::vector<std::string>& params)
{
    std::string r = "(";
    for (size_t i = 0; i < params.size(); ++i) {
        if (i) r += ", ";
        r += params[i];
    }
    return r + ")";
}

static void print_chunk(std::ostream& out, const Chunk& chunk, const std::string& name,
                        std::vector<std::shared_ptr<json::Function>>& pending)
{
    out << "\n=== " << name << " ===\n";
    if (chunk.num_locals > 0)
        out << "  ; slot-based  num_locals=" << chunk.num_locals << "\n";

    for (int i = 0; i < (int)chunk.code.size(); ++i) {
        const auto& instr = chunk.code[i];

        // offset — always right-justified, zero-padded
        out << "  " << std::right << std::setw(4) << std::setfill('0') << i << "  ";

        // opcode name (left-aligned, fixed width)
        const char* op_name = "???";
        switch (instr.opcode) {
            case Opcode::PUSH_NULL:     op_name = "PUSH_NULL";     break;
            case Opcode::PUSH_BOOL:     op_name = "PUSH_BOOL";     break;
            case Opcode::PUSH_INT:      op_name = "PUSH_INT";      break;
            case Opcode::PUSH_DOUBLE:   op_name = "PUSH_DOUBLE";   break;
            case Opcode::PUSH_STRING:   op_name = "PUSH_STRING";   break;
            case Opcode::LOAD:          op_name = "LOAD";          break;
            case Opcode::STORE_LOCAL:   op_name = "STORE_LOCAL";   break;
            case Opcode::STORE_GLOBAL:  op_name = "STORE_GLOBAL";  break;
            case Opcode::ADD:           op_name = "ADD";           break;
            case Opcode::SUB:           op_name = "SUB";           break;
            case Opcode::MUL:           op_name = "MUL";           break;
            case Opcode::DIV:           op_name = "DIV";           break;
            case Opcode::NEG:           op_name = "NEG";           break;
            case Opcode::EQ:            op_name = "EQ";            break;
            case Opcode::NEQ:           op_name = "NEQ";           break;
            case Opcode::LT:            op_name = "LT";            break;
            case Opcode::LTE:           op_name = "LTE";           break;
            case Opcode::GT:            op_name = "GT";            break;
            case Opcode::GTE:           op_name = "GTE";           break;
            case Opcode::NOT:           op_name = "NOT";           break;
            case Opcode::MAKE_ARRAY:    op_name = "MAKE_ARRAY";    break;
            case Opcode::MAKE_OBJECT:   op_name = "MAKE_OBJECT";   break;
            case Opcode::GET_MEMBER:    op_name = "GET_MEMBER";    break;
            case Opcode::SET_MEMBER:    op_name = "SET_MEMBER";    break;
            case Opcode::JUMP:          op_name = "JUMP";          break;
            case Opcode::JUMP_IF_FALSE: op_name = "JUMP_IF_FALSE"; break;
            case Opcode::MAKE_CLOSURE:  op_name = "MAKE_CLOSURE";  break;
            case Opcode::CALL:          op_name = "CALL";          break;
            case Opcode::RETURN:        op_name = "RETURN";        break;
            case Opcode::PRINT:         op_name = "PRINT";         break;
            case Opcode::POP:           op_name = "POP";           break;
            case Opcode::LOAD_SLOT:     op_name = "LOAD_SLOT";     break;
            case Opcode::STORE_SLOT:    op_name = "STORE_SLOT";    break;
        }
        out << std::left << std::setw(16) << std::setfill(' ') << op_name;

        // operand
        std::string operand;
        std::string comment;
        switch (instr.opcode) {
            case Opcode::PUSH_NULL:
                break;
            case Opcode::PUSH_BOOL:
                operand = instr.operand ? "true" : "false";
                break;
            case Opcode::PUSH_INT:
                operand = std::to_string(chunk.int_constants[instr.operand]);
                break;
            case Opcode::PUSH_DOUBLE: {
                std::ostringstream ss;
                ss << chunk.double_constants[instr.operand];
                operand = ss.str();
                break;
            }
            case Opcode::PUSH_STRING:
                operand = quoted(chunk.string_constants[instr.operand]);
                break;
            case Opcode::LOAD:
                operand = quoted(chunk.string_constants[instr.operand]);
                break;
            case Opcode::STORE_LOCAL:
            case Opcode::STORE_GLOBAL:
                operand = quoted(chunk.string_constants[instr.operand]);
                break;
            case Opcode::MAKE_ARRAY:
            case Opcode::MAKE_OBJECT:
                operand = std::to_string(instr.operand);
                break;
            case Opcode::JUMP:
            case Opcode::JUMP_IF_FALSE:
                operand = "→ " + std::to_string(instr.operand);
                break;
            case Opcode::MAKE_CLOSURE: {
                const auto& fn = chunk.function_protos[instr.operand];
                operand = "proto[" + std::to_string(instr.operand) + "]";
                comment = fn->get_text() + params_str(fn->get_params());
                pending.push_back(fn);
                break;
            }
            case Opcode::CALL:
                operand = std::to_string(instr.operand) + " arg(s)";
                break;
            case Opcode::LOAD_SLOT:
            case Opcode::STORE_SLOT:
                operand = "slot[" + std::to_string(instr.operand) + "]";
                break;
            default:
                break;
        }

        out << std::left << std::setw(20) << std::setfill(' ') << operand;
        if (!comment.empty())
            out << "; " << comment;
        out << "\n";
    }
}

void disassemble(std::ostream& out, const Chunk& chunk, const std::string& name)
{
    std::vector<std::shared_ptr<json::Function>> pending;
    print_chunk(out, chunk, name, pending);

    // BFS over all function protos referenced in the chunk tree
    for (size_t i = 0; i < pending.size(); ++i) {
        const auto& fn = pending[i];
        std::string fn_name = "function " + fn->get_text() + params_str(fn->get_params());
        print_chunk(out, fn->get_body(), fn_name, pending);
    }
}
