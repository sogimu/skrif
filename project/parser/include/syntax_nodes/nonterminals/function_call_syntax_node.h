#pragma once

#include "nonterminals/varible_syntax_node.h"
#include <string>
#include <vector>
#include "enums.h"
#include "i_syntax_node.h"

class ISyntaxNodeVisitor;

class NameSyntaxNode;
using NameSyntaxNodeSP = std::shared_ptr< NameSyntaxNode >;

class MemberExpressionSyntaxNode;
using MemberExpressionSyntaxNodeSP = std::shared_ptr< MemberExpressionSyntaxNode >;

class FunctionCallSyntaxNode : public ISyntaxNode, public std::enable_shared_from_this< FunctionCallSyntaxNode >
{
public:
   // enum class Type
   // {
   //    BY_NAME,
   //    BY_OPERATION_EXPRESSION
   // };

   FunctionCallSyntaxNode( const FunctionCallSyntaxNode& function_call_syntax_node );
   // FunctionCallSyntaxNode( const std::string& name );
   // FunctionCallSyntaxNode( const std::string& name, const std::vector< ISyntaxNodeSP >& arguments );
   FunctionCallSyntaxNode( const VaribleSyntaxNodeSP& varible_syntax_node, const std::vector< ISyntaxNodeSP >& arguments_syntax_nodes );
   FunctionCallSyntaxNode( const MemberExpressionSyntaxNodeSP& member_expression_syntax_node, const std::vector< ISyntaxNodeSP >& arguments_syntax_nodes );

   void accept( const std::shared_ptr< ISyntaxNodeVisitor >& visitor ) override;
   bool compare( const ISyntaxNode& node ) const override;

   // Type type() const;
   // std::string name() const;
   std::vector< ISyntaxNodeSP > arguments() const;

   std::vector< LexicalTokens::LexicalToken > lexical_tokens() const override;
private:
    std::vector< LexicalTokens::LexicalToken > mTokens;
    std::string mName;
    // Type mType;
    // size_t mArgumentsNumber;
    // std::pair<std::string, size_t> mSignature;
};

using FunctionCallSyntaxNodeSP = std::shared_ptr< FunctionCallSyntaxNode >;
