#pragma once

#include "i_syntax_node.h"
#include <memory>
#include <string>

class AbstractSyntaxTree;
using AbstractSyntaxTreeSP = std::shared_ptr<AbstractSyntaxTree>;

class ControlFlowGraph
{
public:
   ControlFlowGraph() = default;
   ControlFlowGraph( const ISyntaxNodeSP& cfg_root );
   ControlFlowGraph( const AbstractSyntaxTree& ast );
   ISyntaxNodeSP root() const;
   std::string to_string() const;
   bool operator==( const ControlFlowGraph& cfg ) const;

private:
  ISyntaxNodeSP mRoot;
};

inline std::ostream& operator<<( std::ostream& o, const ControlFlowGraph& rhs )
{
   o << rhs.to_string();
   return o;
}
