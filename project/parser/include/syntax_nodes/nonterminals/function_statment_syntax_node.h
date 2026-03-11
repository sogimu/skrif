#pragma once

#include <string>
#include <vector>
#include "enums.h"
#include "i_syntax_node.h"

class ISyntaxNodeVisitor;

class NameSyntaxNode;
using NameSyntaxNodeSP = std::shared_ptr< NameSyntaxNode >;

class ScopeSyntaxNode;
using ScopeSyntaxNodeSP = std::shared_ptr< ScopeSyntaxNode >;

class FunctionScopeSyntaxNode;
using FunctionScopeSyntaxNodeSP = std::shared_ptr< FunctionScopeSyntaxNode >;

class FunctionStatmentSyntaxNode : public ISyntaxNode, public std::enable_shared_from_this< FunctionStatmentSyntaxNode >
{
public:
   FunctionStatmentSyntaxNode( const FunctionStatmentSyntaxNode& function_statment_syntax_node );
   // FunctionStatmentSyntaxNode( const std::string& name );
   // FunctionStatmentSyntaxNode( const std::string& name, const std::vector< ISyntaxNodeSP >& arguments, const ScopeSyntaxNodeSP& scope );
   FunctionStatmentSyntaxNode( const NameSyntaxNodeSP& name_syntax_node, const std::vector< ISyntaxNodeSP >& arguments, const ScopeSyntaxNodeSP& scope, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens );

   void accept( const std::shared_ptr< ISyntaxNodeVisitor >& visitor ) override;
   bool compare( const ISyntaxNode& node ) const override;

   std::string name() const;
   FunctionScopeSyntaxNodeSP scope() const;
   std::vector< NameSyntaxNodeSP > arguments() const;
   std::vector< LexicalTokens::LexicalToken > lexical_tokens() const override;

private:
   std::string mName;
   std::vector< LexicalTokens::LexicalToken > mTokens;
};

using FunctionStatmentSyntaxNodeSP = std::shared_ptr< FunctionStatmentSyntaxNode >;
