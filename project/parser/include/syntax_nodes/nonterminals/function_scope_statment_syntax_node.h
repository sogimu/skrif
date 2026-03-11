#pragma once

#include "enums.h"
#include "i_syntax_node.h"
#include "scope_statment_syntax_node.h"

#include <vector>

class ISyntaxNodeVisitor;

class FunctionScopeSyntaxNode : public ISyntaxNode, public std::enable_shared_from_this< FunctionScopeSyntaxNode >

{
public:
   FunctionScopeSyntaxNode( const FunctionScopeSyntaxNode& scope );
   FunctionScopeSyntaxNode( const ISyntaxNodeSP& expression, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens );
   FunctionScopeSyntaxNode( const std::string& name, const std::vector< ISyntaxNodeSP >& expressions, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens );
   FunctionScopeSyntaxNode( const ScopeSyntaxNodeSP& scope );
   std::string name() const;
   void accept( const std::shared_ptr< ISyntaxNodeVisitor >& visitor ) override;
   bool compare( const ISyntaxNode& node ) const override;
   std::vector< LexicalTokens::LexicalToken > lexical_tokens() const override;
private:
  std::vector< LexicalTokens::LexicalToken > mTokens;
  std::string mName;
};

using FunctionScopeSyntaxNodeSP = std::shared_ptr< FunctionScopeSyntaxNode >;
