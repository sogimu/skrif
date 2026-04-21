#include "compiler.h"
#include "Function.h"

#include "nonterminals/bin_expr_syntax_node.h"
#include "nonterminals/un_expr_syntax_node.h"
#include "nonterminals/varible_syntax_node.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include "nonterminals/function_statment_syntax_node.h"
#include "nonterminals/function_call_syntax_node.h"
#include "nonterminals/function_scope_statment_syntax_node.h"
#include "nonterminals/if_statment_syntax_node.h"
#include "nonterminals/while_statment_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "nonterminals/print_statment_syntax_node.h"
#include "nonterminals/return_statment_syntax_node.h"
#include "nonterminals/array_syntax_node.h"
#include "nonterminals/object_syntax_node.h"
#include "nonterminals/object_pair_syntax_node.h"
#include "terminals/int_syntax_node.h"
#include "terminals/double_syntax_node.h"
#include "terminals/string_syntax_node.h"
#include "terminals/f_syntax_node.h"
#include "terminals/name_syntax_node.h"
#include <variant>

Chunk& Compiler::cur()
{
    return *chunk_stack_.back();
}

void Compiler::compile_node(const ISyntaxNodeSP& node)
{
    if (node)
        node->accept(shared_from_this());
}

static bool has_nested_functions(const ISyntaxNodeSP& node)
{
    if (!node) return false;
    if (std::dynamic_pointer_cast<FunctionStatmentSyntaxNode>(node)) return true;
    for (const auto& child : node->Children())
        if (has_nested_functions(child)) return true;
    return false;
}

Chunk Compiler::compile(const ISyntaxNodeSP& root)
{
    top_chunk_ = Chunk{};
    chunk_stack_.clear();
    fn_stack_.clear();
    chunk_stack_.push_back(&top_chunk_);
    compile_node(root);
    chunk_stack_.pop_back();
    return std::move(top_chunk_);
}

// ── literals ────────────────────────────────────────────────────────────────

void Compiler::visit(const IntSyntaxNodeSP& node)
{
    cur().emit(Opcode::PUSH_INT, cur().add_int(node->value()));
}

void Compiler::visit(const DoubleSyntaxNodeSP& node)
{
    cur().emit(Opcode::PUSH_DOUBLE, cur().add_double(node->value()));
}

void Compiler::visit(const StringSyntaxNodeSP& node)
{
    cur().emit(Opcode::PUSH_STRING, cur().add_string(node->value()));
}

void Compiler::visit(const FSyntaxNodeSP& node)
{
    auto val = node->value().get();
    if (std::holds_alternative<int>(val)) {
        cur().emit(Opcode::PUSH_INT, cur().add_int(std::get<int>(val)));
    } else if (std::holds_alternative<double>(val)) {
        cur().emit(Opcode::PUSH_DOUBLE, cur().add_double(std::get<double>(val)));
    } else {
        cur().emit(Opcode::PUSH_STRING, cur().add_string(std::get<std::string>(val)));
    }
}

// ── variable ─────────────────────────────────────────────────────────────────

void Compiler::visit(const VaribleSyntaxNodeSP& node)
{
    if (!fn_stack_.empty() && fn_stack_.back().use_slots) {
        auto it = fn_stack_.back().slots.find(node->name());
        if (it != fn_stack_.back().slots.end()) {
            cur().emit(Opcode::LOAD_SLOT, it->second);
            return;
        }
    }
    cur().emit(Opcode::LOAD, cur().add_string(node->name()));
}

// ── expressions ──────────────────────────────────────────────────────────────

void Compiler::visit(const UnExprSyntaxNodeSP& node)
{
    compile_node(node->operator[](0));
    cur().emit(Opcode::NEG);
}

void Compiler::visit(const BinExprSyntaxNodeSP& node)
{
    compile_node(node->operator[](0));
    compile_node(node->operator[](1));

    switch (node->type()) {
        case BinExprSyntaxNode::Type::Addition:    cur().emit(Opcode::ADD);  break;
        case BinExprSyntaxNode::Type::Substruction: cur().emit(Opcode::SUB); break;
        case BinExprSyntaxNode::Type::Multiply:    cur().emit(Opcode::MUL);  break;
        case BinExprSyntaxNode::Type::Division:    cur().emit(Opcode::DIV);  break;
        case BinExprSyntaxNode::Type::Equal:       cur().emit(Opcode::EQ);   break;
        case BinExprSyntaxNode::Type::NotEqual:    cur().emit(Opcode::NEQ);  break;
        case BinExprSyntaxNode::Type::Less:        cur().emit(Opcode::LT);   break;
        case BinExprSyntaxNode::Type::LessEqual:   cur().emit(Opcode::LTE);  break;
        case BinExprSyntaxNode::Type::More:        cur().emit(Opcode::GT);   break;
        case BinExprSyntaxNode::Type::MoreEqual:   cur().emit(Opcode::GTE);  break;
    }
}

// ── member expression (read) ─────────────────────────────────────────────────

void Compiler::visit(const MemberExpressionSyntaxNodeSP& node)
{
    compile_node(node->operator[](0));  // container

    auto key_node = node->operator[](1);
    if (auto name = std::dynamic_pointer_cast<NameSyntaxNode>(key_node))
        cur().emit(Opcode::PUSH_STRING, cur().add_string(name->value()));
    else
        compile_node(key_node);

    cur().emit(Opcode::GET_MEMBER);
}

// ── collections ──────────────────────────────────────────────────────────────

void Compiler::visit(const ArraySyntaxNodeSP& node)
{
    int n = 0;
    for (auto& child : node->Children()) {
        compile_node(child);
        ++n;
    }
    cur().emit(Opcode::MAKE_ARRAY, n);
}

void Compiler::visit(const ObjectSyntaxNodeSP& node)
{
    int n = 0;
    for (auto& child : node->Children()) {
        auto pair = std::dynamic_pointer_cast<ObjectPairSyntaxNode>(child);
        compile_node(pair->key_syntax_node());
        compile_node(pair->value_syntax_node());
        ++n;
    }
    cur().emit(Opcode::MAKE_OBJECT, n);
}

// ── statements ───────────────────────────────────────────────────────────────

void Compiler::visit(const StatmentSyntaxNodeSP& node)
{
    compile_node(node->operator[](0));
}

void Compiler::visit(const ScopeSyntaxNodeSP& node)
{
    for (auto& child : node->Children())
        compile_node(child);
}

void Compiler::visit(const FunctionScopeSyntaxNodeSP& node)
{
    for (auto& child : node->Children())
        compile_node(child);
    // fall-off-end: return null
    cur().emit(Opcode::PUSH_NULL);
    cur().emit(Opcode::RETURN);
}

void Compiler::visit(const PrintStatmentSyntaxNodeSP& node)
{
    compile_node(node->operator[](0));
    cur().emit(Opcode::PRINT);
}

void Compiler::visit(const ReturnStatmentSyntaxNodeSP& node)
{
    compile_node(node->operator[](0));
    cur().emit(Opcode::RETURN);
}

// ── control flow ─────────────────────────────────────────────────────────────

void Compiler::visit(const IfStatmentSyntaxNodeSP& node)
{
    // Parser bug: true_scope()/false_scope() methods index children incorrectly.
    // Actual child layout: [0]=condition, [1]=true_scope, [2]=false_scope (optional)
    compile_node(node->operator[](0));  // condition

    int jif_addr = cur().emit(Opcode::JUMP_IF_FALSE, 0);

    compile_node(node->operator[](1));  // true scope

    bool has_else = (node->Children().size() >= 3);
    if (has_else) {
        int jmp_addr = cur().emit(Opcode::JUMP, 0);
        cur().patch_operand(jif_addr, (int)cur().code.size());
        compile_node(node->operator[](2));  // false scope
        cur().patch_operand(jmp_addr, (int)cur().code.size());
    } else {
        cur().patch_operand(jif_addr, (int)cur().code.size());
    }
}

void Compiler::visit(const WhileStatmentSyntaxNodeSP& node)
{
    int loop_start = (int)cur().code.size();

    compile_node(node->operator[](0));  // condition

    int jif_addr = cur().emit(Opcode::JUMP_IF_FALSE, 0);  // placeholder

    compile_node(node->scope());

    cur().emit(Opcode::JUMP, loop_start);
    cur().patch_operand(jif_addr, (int)cur().code.size());
}

// ── assignment ───────────────────────────────────────────────────────────────

void Compiler::visit(const VaribleAssigmentStatmentSyntaxNodeSP& node)
{
    if (node->target() != nullptr) {
        // Simple variable assignment: name = expr
        compile_node(node->source());
        const std::string& name = node->target()->value();

        if (!fn_stack_.empty() && fn_stack_.back().use_slots) {
            FnContext& ctx = fn_stack_.back();
            auto it = ctx.slots.find(name);
            if (it != ctx.slots.end()) {
                // Known slot variable (re-assignment)
                cur().emit(Opcode::STORE_SLOT, it->second);
                return;
            }
            if (node->context() == VaribleAssigmentStatmentSyntaxNode::Context::LOCAL) {
                // New local declaration: allocate a slot
                int slot = ctx.next_slot++;
                ctx.slots[name] = slot;
                if (slot >= cur().num_locals) cur().num_locals = slot + 1;
                cur().emit(Opcode::STORE_SLOT, slot);
                return;
            }
            // Global assignment to an unknown name: falls through to STORE_GLOBAL
        }

        int name_idx = cur().add_string(name);
        if (node->context() == VaribleAssigmentStatmentSyntaxNode::Context::LOCAL)
            cur().emit(Opcode::STORE_LOCAL, name_idx);
        else
            cur().emit(Opcode::STORE_GLOBAL, name_idx);
    } else {
        // Member assignment: container[key] = expr
        auto lhs = std::dynamic_pointer_cast<MemberExpressionSyntaxNode>(node->operator[](0));
        compile_node(lhs->operator[](0));  // container
        auto key_node = lhs->operator[](1);
        if (auto name = std::dynamic_pointer_cast<NameSyntaxNode>(key_node))
            cur().emit(Opcode::PUSH_STRING, cur().add_string(name->value()));
        else
            compile_node(key_node);
        compile_node(node->source());       // value
        cur().emit(Opcode::SET_MEMBER);
    }
}

// ── functions ────────────────────────────────────────────────────────────────

void Compiler::visit(const FunctionStatmentSyntaxNodeSP& node)
{
    // Collect parameter names (arguments() includes scope as last null element — filter it)
    std::vector<std::string> params;
    for (auto& arg : node->arguments())
        if (arg) params.push_back(arg->value());

    // Pre-scan body to decide slot eligibility
    const auto& body_node = node->operator[](node->Children().size() - 1);
    bool use_slots = !has_nested_functions(body_node);

    // Set up per-function slot context
    FnContext ctx;
    ctx.use_slots = use_slots;
    if (use_slots) {
        int slot = 0;
        for (const auto& p : params)
            ctx.slots[p] = slot++;
        ctx.next_slot = slot;
    }

    auto body_chunk = std::make_shared<Chunk>();
    // num_locals > 0 signals slot-based dispatch in the VM; only set for slot functions
    body_chunk->num_locals = use_slots ? (int)params.size() : 0;

    fn_stack_.push_back(std::move(ctx));
    chunk_stack_.push_back(body_chunk.get());

    compile_node(body_node);
    cur().emit(Opcode::PUSH_NULL);
    cur().emit(Opcode::RETURN);

    chunk_stack_.pop_back();
    fn_stack_.pop_back();

    // Create a proto (closure set to nullptr at compile time; VM fills it at MAKE_CLOSURE)
    auto proto = std::make_shared<json::Function>(nullptr, node->name(), params, body_chunk);
    int proto_idx = cur().add_function_proto(proto);

    cur().emit(Opcode::MAKE_CLOSURE, proto_idx);

    // Function name is stored in the OUTER scope.
    // The outer scope cannot be a slot-function if it contains a nested function definition,
    // so STORE_LOCAL is always correct here.
    int name_idx = cur().add_string(node->name());
    cur().emit(Opcode::STORE_LOCAL, name_idx);
}

void Compiler::visit(const FunctionCallSyntaxNodeSP& node)
{
    // Parser stores args first, callee last: children = [arg0, arg1, ..., callee]
    int n_children = (int)node->Children().size();
    compile_node(node->operator[](n_children - 1));  // callee
    for (int i = 0; i < n_children - 1; ++i)
        compile_node(node->operator[](i));            // args in order
    cur().emit(Opcode::CALL, n_children - 1);
}
