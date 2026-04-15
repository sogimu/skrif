#pragma once

#include "enums.h"
#include "i_syntax_node.h"
#include "i_terminal_syntax_node.h"

#include <memory>

class ISyntaxNodeVisitor;

class DotSyntaxNode : public ITerminalSyntaxNode, public std::enable_shared_from_this< DotSyntaxNode >
{
public:
   DotSyntaxNode( const DotSyntaxNode& node );

   DotSyntaxNode( const LexicalTokens::LexicalToken& token );

   void accept( const std::shared_ptr< ISyntaxNodeVisitor >& visitor ) override;

   bool compare( const ISyntaxNode& node ) const override;
};

using DotSyntaxNodeSP = std::shared_ptr< DotSyntaxNode >;
