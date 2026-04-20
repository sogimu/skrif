#include "vm.h"
#include "chunk.h"
#include "Function.h"
#include "EnvScope.h"

#include <stdexcept>
#include <cstdio>
#include <iostream>

// ── cycle-breaker (same logic as tree-walking interpreter) ───────────────────

static void severParentChainCycle(std::shared_ptr<EnvScope> scope, EnvScope* target)
{
    auto current = scope;
    while (current) {
        auto parent = current->getParent();
        if (parent.get() == target) {
            current->set_parent(nullptr);
            return;
        }
        current = parent;
    }
}

static void breakCyclesInJson(Json& value, EnvScope* dying_scope)
{
    if (value.is_function()) {
        auto& f = value.get_function();
        auto closure = f.getScope();
        if (!closure) return;
        if (closure.get() == dying_scope)
            f.clearScope();
        else
            severParentChainCycle(closure, dying_scope);
    } else if (value.is_array()) {
        for (auto& el : value.get_array())
            breakCyclesInJson(el, dying_scope);
    } else if (value.is_object()) {
        for (auto& [k, v] : value.get_object())
            breakCyclesInJson(v, dying_scope);
    }
}

static void breakCycles(EnvScope* dying_scope)
{
    if (!dying_scope) return;
    for (auto it = dying_scope->begin(EnvScope::VaribleType::Local);
         it != dying_scope->end(EnvScope::VaribleType::Local); ++it)
        breakCyclesInJson(it->second, dying_scope);
}

// ── stack helpers ────────────────────────────────────────────────────────────

void VM::push(Json v) { stack_.push_back(std::move(v)); }

Json VM::pop()
{
    Json v = std::move(stack_.back());
    stack_.pop_back();
    return v;
}

Json& VM::peek() { return stack_.back(); }

// ── dispatch loop ─────────────────────────────────────────────────────────────

Json VM::run(const Chunk& top_chunk, std::shared_ptr<EnvScope> global_env)
{
    stack_.clear();
    frames_.clear();

    frames_.push_back({ &top_chunk, 0, 0, global_env });

    while (!frames_.empty()) {
        auto& frame = frames_.back();

        if (frame.ip >= (int)frame.chunk->code.size()) {
            // Top-level ran off end — return top of stack (or null)
            break;
        }

        const Instruction instr = frame.chunk->code[frame.ip++];

        switch (instr.opcode) {

        case Opcode::PUSH_NULL:
            push(Json{});
            break;

        case Opcode::PUSH_BOOL:
            push(Json{(bool)instr.operand});
            break;

        case Opcode::PUSH_INT:
            push(Json{frame.chunk->int_constants[instr.operand]});
            break;

        case Opcode::PUSH_DOUBLE:
            push(Json{frame.chunk->double_constants[instr.operand]});
            break;

        case Opcode::PUSH_STRING:
            push(Json{frame.chunk->string_constants[instr.operand]});
            break;

        case Opcode::LOAD: {
            const std::string& name = frame.chunk->string_constants[instr.operand];
            push(frame.env->operator[](name));
            break;
        }

        case Opcode::STORE_LOCAL: {
            const std::string& name = frame.chunk->string_constants[instr.operand];
            Json val = pop();
            frame.env->write(name, val, EnvScope::VaribleType::Local);
            break;
        }

        case Opcode::STORE_GLOBAL: {
            const std::string& name = frame.chunk->string_constants[instr.operand];
            Json val = pop();
            frame.env->write(name, val, EnvScope::VaribleType::Global);
            break;
        }

        case Opcode::ADD: { Json r = pop(); Json l = pop(); push(l + r); break; }
        case Opcode::SUB: { Json r = pop(); Json l = pop(); push(l - r); break; }
        case Opcode::MUL: { Json r = pop(); Json l = pop(); push(l * r); break; }
        case Opcode::DIV: { Json r = pop(); Json l = pop(); push(l / r); break; }

        case Opcode::NEG: {
            Json v = pop();
            if (v.is_int())    push(Json{-v.get_int()});
            else if (v.is_double()) push(Json{-v.get_double()});
            else push(Json{});
            break;
        }

        case Opcode::NOT: {
            Json v = pop();
            if (v.is_bool()) push(Json{!v.get_bool()});
            else push(Json{});
            break;
        }

        case Opcode::EQ:  { Json r = pop(); Json l = pop(); push(Json{l == r}); break; }
        case Opcode::NEQ: { Json r = pop(); Json l = pop(); push(Json{l != r}); break; }
        case Opcode::LT:  { Json r = pop(); Json l = pop(); push(Json{l < r});  break; }
        case Opcode::LTE: { Json r = pop(); Json l = pop(); push(Json{l <= r}); break; }
        case Opcode::GT:  { Json r = pop(); Json l = pop(); push(Json{l > r});  break; }
        case Opcode::GTE: { Json r = pop(); Json l = pop(); push(Json{l >= r}); break; }

        case Opcode::MAKE_ARRAY: {
            int n = instr.operand;
            std::vector<Json> elems(n);
            for (int i = n - 1; i >= 0; --i)
                elems[i] = pop();
            push(Json{elems});
            break;
        }

        case Opcode::MAKE_OBJECT: {
            int n = instr.operand;
            std::map<std::string, Json> obj;
            for (int i = 0; i < n; ++i) {
                Json val = pop();
                Json key = pop();
                obj[key.get_string()] = std::move(val);
            }
            push(Json{obj});
            break;
        }

        case Opcode::GET_MEMBER: {
            Json key = pop();
            Json container = pop();
            if (container.is_array()) {
                auto& arr = container.get_array();
                int idx = key.get_int();
                if (idx < 0 || idx >= (int)arr.size())
                    push(Json{});
                else
                    push(arr[idx]);
            } else if (container.is_object()) {
                auto& obj = container.get_object();
                auto it = obj.find(key.get_string());
                push(it != obj.end() ? it->second : Json{});
            } else {
                push(Json{});
            }
            break;
        }

        case Opcode::SET_MEMBER: {
            Json val       = pop();
            Json key       = pop();
            Json container = pop();
            if (container.is_array()) {
                int idx = key.get_int();
                auto& arr = container.get_array();
                if (idx >= (int)arr.size())
                    arr.resize(idx + 1);
                arr[idx] = std::move(val);
            } else if (container.is_object()) {
                container.get_object()[key.get_string()] = std::move(val);
            }
            break;
        }

        case Opcode::JUMP:
            frame.ip = instr.operand;
            break;

        case Opcode::JUMP_IF_FALSE: {
            Json cond = pop();
            bool falsy = cond.is_null()
                      || (cond.is_bool()   && !cond.get_bool())
                      || (cond.is_int()    && cond.get_int() == 0)
                      || (cond.is_double() && cond.get_double() == 0.0);
            if (falsy)
                frame.ip = instr.operand;
            break;
        }

        case Opcode::MAKE_CLOSURE: {
            const auto& proto = frame.chunk->function_protos[instr.operand];
            auto fn = std::make_shared<json::Function>(
                frame.env,
                proto->get_text(),
                std::vector<std::string>(proto->get_params()),
                proto->get_body_chunk());
            push(Json{*fn});
            break;
        }

        case Opcode::CALL: {
            int n_args = instr.operand;

            // Pop arguments (they were pushed left-to-right, so last arg is on top)
            std::vector<Json> args(n_args);
            for (int i = n_args - 1; i >= 0; --i)
                args[i] = pop();

            Json callee = pop();
            if (!callee.is_function())
                throw std::runtime_error("CALL: not a function");

            const json::Function& fn = callee.get_function();
            const auto& params = fn.get_params();

            // New scope rooted at closure
            auto new_env = std::make_shared<EnvScope>(fn.getScope());
            for (int i = 0; i < (int)params.size(); ++i)
                new_env->write(params[i], args[i], EnvScope::VaribleType::Local);

            int base = (int)stack_.size();
            frames_.push_back({ &fn.get_body(), 0, base, new_env });
            break;
        }

        case Opcode::RETURN: {
            Json return_val = pop();
            breakCycles(frame.env.get());
            int base = frame.base_stack_size;
            frames_.pop_back();

            if (frames_.empty()) {
                return return_val;
            }

            // Restore stack to before CALL, then push return value
            stack_.resize(base);
            push(std::move(return_val));
            break;
        }

        case Opcode::PRINT: {
            Json val = pop();
            if (val.is_string())
                std::printf("%s\n", val.get_string().c_str());
            else if (val.is_int())
                std::printf("%d\n", val.get_int());
            else if (val.is_double())
                std::printf("%f\n", val.get_double());
            else if (val.is_bool())
                std::printf("%s\n", val.get_bool() ? "true" : "false");
            else if (val.is_function())
                std::printf("%s\n", val.get_function().get_text().c_str());
            else
                std::cout << val << "\n";
            break;
        }

        case Opcode::POP:
            pop();
            break;
        }
    }

    // Top-level ran off end without explicit RETURN.
    // The top-level frame's env still lives on `frames_` — break cycles before it dies.
    while (!frames_.empty()) {
        breakCycles(frames_.back().env.get());
        frames_.pop_back();
    }
    if (!stack_.empty())
        return pop();
    return Json{};
}
