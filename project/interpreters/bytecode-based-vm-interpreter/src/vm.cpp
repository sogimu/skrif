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
    stack_.reserve(256);
    frames_.clear();
    frames_.reserve(256);

    frames_.push_back({ &top_chunk, 0, 0, global_env, {} });

// ── computed-goto fast path (GCC / Clang) ────────────────────────────────────
#ifdef __GNUC__
    static const void* const dispatch_table[] = {
        &&op_PUSH_NULL,     // 0
        &&op_PUSH_BOOL,     // 1
        &&op_PUSH_INT,      // 2
        &&op_PUSH_DOUBLE,   // 3
        &&op_PUSH_STRING,   // 4
        &&op_LOAD,          // 5
        &&op_STORE_LOCAL,   // 6
        &&op_STORE_GLOBAL,  // 7
        &&op_ADD,           // 8
        &&op_SUB,           // 9
        &&op_MUL,           // 10
        &&op_DIV,           // 11
        &&op_NEG,           // 12
        &&op_EQ,            // 13
        &&op_NEQ,           // 14
        &&op_LT,            // 15
        &&op_LTE,           // 16
        &&op_GT,            // 17
        &&op_GTE,           // 18
        &&op_NOT,           // 19
        &&op_MAKE_ARRAY,    // 20
        &&op_MAKE_OBJECT,   // 21
        &&op_GET_MEMBER,    // 22
        &&op_SET_MEMBER,    // 23
        &&op_JUMP,          // 24
        &&op_JUMP_IF_FALSE, // 25
        &&op_MAKE_CLOSURE,  // 26
        &&op_CALL,          // 27
        &&op_RETURN,        // 28
        &&op_PRINT,         // 29
        &&op_POP,           // 30
        &&op_LOAD_SLOT,     // 31
        &&op_STORE_SLOT,    // 32
    };

    Instruction gt_instr;
    CallFrame* gt_frame;

#define DISPATCH() \
    do { \
        if (frames_.empty()) goto gt_end; \
        gt_frame = &frames_.back(); \
        if (gt_frame->ip >= (int)gt_frame->chunk->code.size()) goto gt_end; \
        gt_instr = gt_frame->chunk->code[gt_frame->ip++]; \
        goto *dispatch_table[(int)gt_instr.opcode]; \
    } while(0)

    DISPATCH();

    op_PUSH_NULL:   push(Json{});                                                           DISPATCH();
    op_PUSH_BOOL:   push(Json{(bool)gt_instr.operand});                                     DISPATCH();
    op_PUSH_INT:    push(Json{gt_frame->chunk->int_constants[gt_instr.operand]});           DISPATCH();
    op_PUSH_DOUBLE: push(Json{gt_frame->chunk->double_constants[gt_instr.operand]});        DISPATCH();
    op_PUSH_STRING: push(Json{gt_frame->chunk->string_constants[gt_instr.operand]});        DISPATCH();

    op_LOAD: {
        push(gt_frame->env->operator[](gt_frame->chunk->string_constants[gt_instr.operand]));
        DISPATCH();
    }
    op_STORE_LOCAL: {
        { Json val = pop(); gt_frame->env->write(gt_frame->chunk->string_constants[gt_instr.operand], val, EnvScope::VaribleType::Local); }
        DISPATCH();
    }
    op_STORE_GLOBAL: {
        { Json val = pop(); gt_frame->env->write(gt_frame->chunk->string_constants[gt_instr.operand], val, EnvScope::VaribleType::Global); }
        DISPATCH();
    }

    op_ADD: { { Json r = pop(); Json l = pop(); push(l + r); } DISPATCH(); }
    op_SUB: { { Json r = pop(); Json l = pop(); push(l - r); } DISPATCH(); }
    op_MUL: { { Json r = pop(); Json l = pop(); push(l * r); } DISPATCH(); }
    op_DIV: { { Json r = pop(); Json l = pop(); push(l / r); } DISPATCH(); }

    op_NEG: {
        {
            Json v = pop();
            if (v.is_int())         push(Json{-v.get_int()});
            else if (v.is_double()) push(Json{-v.get_double()});
            else                    push(Json{});
        }
        DISPATCH();
    }
    op_NOT: {
        { Json v = pop(); push(v.is_bool() ? Json{!v.get_bool()} : Json{}); }
        DISPATCH();
    }

    op_EQ:  { { Json r = pop(); Json l = pop(); push(Json{l == r}); } DISPATCH(); }
    op_NEQ: { { Json r = pop(); Json l = pop(); push(Json{l != r}); } DISPATCH(); }
    op_LT:  { { Json r = pop(); Json l = pop(); push(Json{l <  r}); } DISPATCH(); }
    op_LTE: { { Json r = pop(); Json l = pop(); push(Json{l <= r}); } DISPATCH(); }
    op_GT:  { { Json r = pop(); Json l = pop(); push(Json{l >  r}); } DISPATCH(); }
    op_GTE: { { Json r = pop(); Json l = pop(); push(Json{l >= r}); } DISPATCH(); }

    op_MAKE_ARRAY: {
        {
            int n = gt_instr.operand;
            int base = (int)stack_.size() - n;
            std::vector<Json> elems(
                std::make_move_iterator(stack_.begin() + base),
                std::make_move_iterator(stack_.end()));
            stack_.resize(base);
            push(Json{std::move(elems)});
        }
        DISPATCH();
    }
    op_MAKE_OBJECT: {
        {
            int n = gt_instr.operand;
            std::map<std::string, Json> obj;
            for (int i = 0; i < n; ++i) {
                Json val = pop(); Json key = pop();
                obj[key.get_string()] = std::move(val);
            }
            push(Json{std::move(obj)});
        }
        DISPATCH();
    }

    op_GET_MEMBER: {
        {
            Json key = pop();
            Json container = pop();
            if (container.is_array()) {
                auto& arr = container.get_array();
                int idx = key.get_int();
                push((idx >= 0 && idx < (int)arr.size()) ? arr[idx] : Json{});
            } else if (container.is_object()) {
                auto& obj = container.get_object();
                auto it = obj.find(key.get_string());
                push(it != obj.end() ? it->second : Json{});
            } else {
                push(Json{});
            }
        }
        DISPATCH();
    }
    op_SET_MEMBER: {
        {
            Json val = pop(); Json key = pop(); Json container = pop();
            if (container.is_array()) {
                int idx = key.get_int();
                auto& arr = container.get_array();
                if (idx >= (int)arr.size()) arr.resize(idx + 1);
                arr[idx] = std::move(val);
            } else if (container.is_object()) {
                container.get_object()[key.get_string()] = std::move(val);
            }
        }
        DISPATCH();
    }

    op_JUMP:
        gt_frame->ip = gt_instr.operand;
        DISPATCH();

    op_JUMP_IF_FALSE: {
        {
            Json cond = pop();
            bool falsy = cond.is_null()
                      || (cond.is_bool()   && !cond.get_bool())
                      || (cond.is_int()    && cond.get_int() == 0)
                      || (cond.is_double() && cond.get_double() == 0.0);
            if (falsy) gt_frame->ip = gt_instr.operand;
        }
        DISPATCH();
    }

    op_MAKE_CLOSURE: {
        {
            const auto& proto = gt_frame->chunk->function_protos[gt_instr.operand];
            auto fn = std::make_shared<json::Function>(
                gt_frame->env,
                proto->get_text(),
                std::vector<std::string>(proto->get_params()),
                proto->get_body_chunk());
            push(Json{*fn});
        }
        DISPATCH();
    }

    op_CALL: {
        {
            int n_args = gt_instr.operand;
            int args_base = (int)stack_.size() - n_args;
            std::vector<Json> args(
                std::make_move_iterator(stack_.begin() + args_base),
                std::make_move_iterator(stack_.end()));
            stack_.resize(args_base);

            Json callee = pop();
            if (!callee.is_function()) throw std::runtime_error("CALL: not a function");

            const json::Function& fn = callee.get_function();
            const Chunk& body = fn.get_body();
            const auto& params = fn.get_params();

            auto new_env = std::make_shared<EnvScope>(fn.getScope());
            int base = (int)stack_.size();
            CallFrame nf{ &body, 0, base, new_env, {} };

            if (body.num_locals > 0) {
                nf.locals.resize(body.num_locals);
                for (int i = 0; i < (int)params.size(); ++i)
                    nf.locals[i] = std::move(args[i]);
            } else {
                for (int i = 0; i < (int)params.size(); ++i)
                    new_env->write(params[i], args[i], EnvScope::VaribleType::Local);
            }
            frames_.push_back(std::move(nf));
        }
        DISPATCH();
    }

    op_RETURN: {
        Json return_val = pop();
        if (gt_frame->env->has_closures)
            breakCycles(gt_frame->env.get());
        int base = gt_frame->base_stack_size;
        frames_.pop_back();
        if (frames_.empty()) return return_val;
        stack_.resize(base);
        push(std::move(return_val));
        DISPATCH();
    }

    op_PRINT: {
        {
            Json val = pop();
            if      (val.is_string())   std::printf("%s\n",  val.get_string().c_str());
            else if (val.is_int())      std::printf("%d\n",  val.get_int());
            else if (val.is_double())   std::printf("%f\n",  val.get_double());
            else if (val.is_bool())     std::printf("%s\n",  val.get_bool() ? "true" : "false");
            else if (val.is_function()) std::printf("%s\n",  val.get_function().get_text().c_str());
            else                        std::cout << val << "\n";
        }
        DISPATCH();
    }

    op_POP:        { pop(); }                                     DISPATCH();
    op_LOAD_SLOT:  push(gt_frame->locals[gt_instr.operand]);      DISPATCH();
    op_STORE_SLOT: { gt_frame->locals[gt_instr.operand] = pop(); } DISPATCH();

gt_end:
#undef DISPATCH

// ── switch-based fallback (non-GCC compilers) ────────────────────────────────
#else
    while (!frames_.empty()) {
        auto& frame = frames_.back();

        if (frame.ip >= (int)frame.chunk->code.size())
            break;

        const Instruction instr = frame.chunk->code[frame.ip++];

        switch (instr.opcode) {

        case Opcode::PUSH_NULL:   push(Json{});                                              break;
        case Opcode::PUSH_BOOL:   push(Json{(bool)instr.operand});                          break;
        case Opcode::PUSH_INT:    push(Json{frame.chunk->int_constants[instr.operand]});    break;
        case Opcode::PUSH_DOUBLE: push(Json{frame.chunk->double_constants[instr.operand]}); break;
        case Opcode::PUSH_STRING: push(Json{frame.chunk->string_constants[instr.operand]}); break;

        case Opcode::LOAD: {
            push(frame.env->operator[](frame.chunk->string_constants[instr.operand]));
            break;
        }
        case Opcode::STORE_LOCAL: {
            Json val = pop();
            frame.env->write(frame.chunk->string_constants[instr.operand], val, EnvScope::VaribleType::Local);
            break;
        }
        case Opcode::STORE_GLOBAL: {
            Json val = pop();
            frame.env->write(frame.chunk->string_constants[instr.operand], val, EnvScope::VaribleType::Global);
            break;
        }

        case Opcode::ADD: { Json r = pop(); Json l = pop(); push(l + r); break; }
        case Opcode::SUB: { Json r = pop(); Json l = pop(); push(l - r); break; }
        case Opcode::MUL: { Json r = pop(); Json l = pop(); push(l * r); break; }
        case Opcode::DIV: { Json r = pop(); Json l = pop(); push(l / r); break; }

        case Opcode::NEG: {
            Json v = pop();
            if (v.is_int())         push(Json{-v.get_int()});
            else if (v.is_double()) push(Json{-v.get_double()});
            else                    push(Json{});
            break;
        }
        case Opcode::NOT: {
            Json v = pop();
            push(v.is_bool() ? Json{!v.get_bool()} : Json{});
            break;
        }

        case Opcode::EQ:  { Json r = pop(); Json l = pop(); push(Json{l == r}); break; }
        case Opcode::NEQ: { Json r = pop(); Json l = pop(); push(Json{l != r}); break; }
        case Opcode::LT:  { Json r = pop(); Json l = pop(); push(Json{l <  r}); break; }
        case Opcode::LTE: { Json r = pop(); Json l = pop(); push(Json{l <= r}); break; }
        case Opcode::GT:  { Json r = pop(); Json l = pop(); push(Json{l >  r}); break; }
        case Opcode::GTE: { Json r = pop(); Json l = pop(); push(Json{l >= r}); break; }

        case Opcode::MAKE_ARRAY: {
            int n = instr.operand;
            int base = (int)stack_.size() - n;
            std::vector<Json> elems(
                std::make_move_iterator(stack_.begin() + base),
                std::make_move_iterator(stack_.end()));
            stack_.resize(base);
            push(Json{std::move(elems)});
            break;
        }
        case Opcode::MAKE_OBJECT: {
            int n = instr.operand;
            std::map<std::string, Json> obj;
            for (int i = 0; i < n; ++i) {
                Json val = pop(); Json key = pop();
                obj[key.get_string()] = std::move(val);
            }
            push(Json{std::move(obj)});
            break;
        }

        case Opcode::GET_MEMBER: {
            Json key = pop();
            Json container = pop();
            if (container.is_array()) {
                auto& arr = container.get_array();
                int idx = key.get_int();
                push((idx >= 0 && idx < (int)arr.size()) ? arr[idx] : Json{});
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
            Json val = pop(); Json key = pop(); Json container = pop();
            if (container.is_array()) {
                int idx = key.get_int();
                auto& arr = container.get_array();
                if (idx >= (int)arr.size()) arr.resize(idx + 1);
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
            if (falsy) frame.ip = instr.operand;
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
            int args_base = (int)stack_.size() - n_args;
            std::vector<Json> args(
                std::make_move_iterator(stack_.begin() + args_base),
                std::make_move_iterator(stack_.end()));
            stack_.resize(args_base);

            Json callee = pop();
            if (!callee.is_function()) throw std::runtime_error("CALL: not a function");

            const json::Function& fn = callee.get_function();
            const Chunk& body = fn.get_body();
            const auto& params = fn.get_params();

            auto new_env = std::make_shared<EnvScope>(fn.getScope());
            int base = (int)stack_.size();
            CallFrame nf{ &body, 0, base, new_env, {} };

            if (body.num_locals > 0) {
                nf.locals.resize(body.num_locals);
                for (int i = 0; i < (int)params.size(); ++i)
                    nf.locals[i] = std::move(args[i]);
            } else {
                for (int i = 0; i < (int)params.size(); ++i)
                    new_env->write(params[i], args[i], EnvScope::VaribleType::Local);
            }
            frames_.push_back(std::move(nf));
            break;
        }

        case Opcode::RETURN: {
            Json return_val = pop();
            if (frame.env->has_closures)
                breakCycles(frame.env.get());
            int base = frame.base_stack_size;
            frames_.pop_back();
            if (frames_.empty()) return return_val;
            stack_.resize(base);
            push(std::move(return_val));
            break;
        }

        case Opcode::PRINT: {
            Json val = pop();
            if      (val.is_string())   std::printf("%s\n",  val.get_string().c_str());
            else if (val.is_int())      std::printf("%d\n",  val.get_int());
            else if (val.is_double())   std::printf("%f\n",  val.get_double());
            else if (val.is_bool())     std::printf("%s\n",  val.get_bool() ? "true" : "false");
            else if (val.is_function()) std::printf("%s\n",  val.get_function().get_text().c_str());
            else                        std::cout << val << "\n";
            break;
        }

        case Opcode::POP:        pop();                               break;
        case Opcode::LOAD_SLOT:  push(frame.locals[instr.operand]);  break;
        case Opcode::STORE_SLOT: frame.locals[instr.operand] = pop(); break;
        }
    }
#endif

    // Top-level ran off end without explicit RETURN — break cycles, return top of stack.
    while (!frames_.empty()) {
        if (frames_.back().env->has_closures)
            breakCycles(frames_.back().env.get());
        frames_.pop_back();
    }
    if (!stack_.empty())
        return pop();
    return Json{};
}
