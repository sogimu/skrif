#pragma once

#include "enums.h"
#include "i_syntax_node.h"

#include <vector>

class BinExprSyntaxNode : public ISyntaxNode, public std::enable_shared_from_this< BinExprSyntaxNode >
{
public:
  enum class Type
  {
    Addition,
    Substruction,
    Multiply,
    Division,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    More,
    MoreEqual
  };
  BinExprSyntaxNode( const BinExprSyntaxNode& bin_expr_syntax_node );
  BinExprSyntaxNode( const Type& type );
  BinExprSyntaxNode( const Type& type, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens );
  BinExprSyntaxNode( const Type& type, const ISyntaxNodeSP& arg0, const ISyntaxNodeSP& arg1, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens );
  bool compare( const ISyntaxNode& node ) const override;
  void accept( const std::shared_ptr< ISyntaxNodeVisitor >& visitor ) override;
  // ISyntaxNodeSP& add_back( const ISyntaxNodeSP& child ) override;
  Type type() const;

private:
    Type mType;
    std::vector< LexicalTokens::LexicalToken > lexical_tokens() const override;
    void add_lexical_token(const LexicalTokens::LexicalToken& token);

private:
  std::vector< LexicalTokens::LexicalToken > mTokens;
};

using BinExprSyntaxNodeSP = std::shared_ptr< BinExprSyntaxNode >;
using BinExprSyntaxNodeCSP = std::shared_ptr< const BinExprSyntaxNode >;
