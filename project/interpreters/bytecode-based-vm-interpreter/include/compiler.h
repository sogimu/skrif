#pragma once
#include "chunk.h"
#include "i_syntax_node.h"
#include "i_syntax_node_visitor.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class Compiler : public ISyntaxNodeVisitor, public std::enable_shared_from_this<Compiler> {
public:
    Chunk compile(const ISyntaxNodeSP& root);

private:
    Chunk& cur();
    void   compile_node(const ISyntaxNodeSP& node);

    struct FnContext {
        std::unordered_map<std::string, int> slots;
        int  next_slot = 0;
        bool use_slots = false;
    };

    std::vector<Chunk*>                 chunk_stack_;
    std::vector<FnContext>              fn_stack_;
    Chunk                               top_chunk_;

    // ISyntaxNodeVisitor — terminals (noop)
    void visit(const BolSyntaxNodeSP&)                override {}
    void visit(const EolSyntaxNodeSP&)                override {}
    void visit(const PlusSyntaxNodeSP&)               override {}
    void visit(const MinusSyntaxNodeSP&)              override {}
    void visit(const AsteriskSyntaxNodeSP&)           override {}
    void visit(const SlashSyntaxNodeSP&)              override {}
    void visit(const SemicolonSyntaxNodeSP&)          override {}
    void visit(const ColonSyntaxNodeSP&)              override {}
    void visit(const DotSyntaxNodeSP&)                override {}
    void visit(const VarSyntaxNodeSP&)                override {}
    void visit(const OpenCurlyBracketSyntaxNodeSP&)   override {}
    void visit(const CloseCurlyBracketSyntaxNodeSP&)  override {}
    void visit(const OpenCircleBracketSyntaxNodeSP&)  override {}
    void visit(const CloseCircleBracketSyntaxNodeSP&) override {}
    void visit(const OpenSquareBracketSyntaxNodeSP&)  override {}
    void visit(const CloseSquareBracketSyntaxNodeSP&) override {}
    void visit(const EqualSyntaxNodeSP&)              override {}
    void visit(const NotSyntaxNodeSP&)                override {}
    void visit(const LessSyntaxNodeSP&)               override {}
    void visit(const MoreSyntaxNodeSP&)               override {}
    void visit(const IfSyntaxNodeSP&)                 override {}
    void visit(const ElseSyntaxNodeSP&)               override {}
    void visit(const WhileSyntaxNodeSP&)              override {}
    void visit(const GotoSyntaxNodeSP&)               override {}
    void visit(const FunctionSyntaxNodeSP&)           override {}
    void visit(const ReturnSyntaxNodeSP&)             override {}
    void visit(const PrintSyntaxNodeSP&)              override {}
    void visit(const CommaSyntaxNodeSP&)              override {}
    void visit(const NameSyntaxNodeSP&)               override {}
    void visit(const ObjectPairSyntaxNodeSP&)         override {}

    // ISyntaxNodeVisitor — code-generating nodes
    void visit(const IntSyntaxNodeSP&)                            override;
    void visit(const DoubleSyntaxNodeSP&)                         override;
    void visit(const StringSyntaxNodeSP&)                         override;
    void visit(const FSyntaxNodeSP&)                              override;
    void visit(const VaribleSyntaxNodeSP&)                        override;
    void visit(const UnExprSyntaxNodeSP&)                         override;
    void visit(const BinExprSyntaxNodeSP&)                        override;
    void visit(const MemberExpressionSyntaxNodeSP&)               override;
    void visit(const ArraySyntaxNodeSP&)                          override;
    void visit(const ObjectSyntaxNodeSP&)                         override;
    void visit(const StatmentSyntaxNodeSP&)                       override;
    void visit(const ScopeSyntaxNodeSP&)                          override;
    void visit(const FunctionScopeSyntaxNodeSP&)                  override;
    void visit(const PrintStatmentSyntaxNodeSP&)                  override;
    void visit(const ReturnStatmentSyntaxNodeSP&)                 override;
    void visit(const IfStatmentSyntaxNodeSP&)                     override;
    void visit(const WhileStatmentSyntaxNodeSP&)                  override;
    void visit(const VaribleAssigmentStatmentSyntaxNodeSP&)       override;
    void visit(const FunctionStatmentSyntaxNodeSP&)               override;
    void visit(const FunctionCallSyntaxNodeSP&)                   override;
};
