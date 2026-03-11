#include "nonterminals/function_scope_statment_syntax_node.h"

#include "i_syntax_node_visitor.h"
#include "make_shallow_syntax_node_copy.h"
#include "syntax_node_empty_visitor.h"

FunctionScopeSyntaxNode::FunctionScopeSyntaxNode( const FunctionScopeSyntaxNode& scope )
   : ISyntaxNode{ Token_Type::FUNCTION_SCOPE_STATMENT }
{
   mTokens = scope.lexical_tokens();
}

FunctionScopeSyntaxNode::FunctionScopeSyntaxNode( const ISyntaxNodeSP& expression, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens )
   : ISyntaxNode{ Token_Type::FUNCTION_SCOPE_STATMENT }
{
   mTokens = lexical_tokens;
   add_back( expression );
}

FunctionScopeSyntaxNode::FunctionScopeSyntaxNode( const std::string& name, const std::vector< ISyntaxNodeSP >& expressions, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens )
   : ISyntaxNode{ Token_Type::FUNCTION_SCOPE_STATMENT }
{
   mName = name;
   mTokens = lexical_tokens;
   for( const auto& expression : expressions )
      add_back( expression );
}

FunctionScopeSyntaxNode::FunctionScopeSyntaxNode( const ScopeSyntaxNodeSP& scope )
   : ISyntaxNode{ Token_Type::FUNCTION_SCOPE_STATMENT }
{
   mTokens = scope->lexical_tokens();
   for( const auto& expression : *scope )
      add_back( expression );
}

void FunctionScopeSyntaxNode::accept( const ISyntaxNodeVisitorSP& visitor )
{
   visitor->visit( shared_from_this() );
}

std::vector< LexicalTokens::LexicalToken > FunctionScopeSyntaxNode::lexical_tokens() const
{ 
    return mTokens; 
}

std::string FunctionScopeSyntaxNode::name() const
{
    return mName;
}

bool FunctionScopeSyntaxNode::compare( const ISyntaxNode& node ) const
{
   bool is_equal = true;
   SyntaxNodeEmptyVisitor::Handlers handlers;
   handlers.function_scope_statment_syntax_node = [ this, &is_equal ]( const FunctionScopeSyntaxNodeSP& node )
   {
      if( node->Children().size() != this->Children().size() )
         return;
      if( node->lexical_tokens() != this->lexical_tokens() )
         return;
      for( size_t i = 0; i < Children().size(); ++i )
      {
         const auto& lft_child = ( *this )[ i ];
         const auto& rht_child = ( *node )[ i ];
         if( !lft_child->compare( *rht_child ) )
         {
            return;
         }
      }
      is_equal = true;
   };
   const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
   const_cast< ISyntaxNode& >( node ).accept( visitor );

   return is_equal;
}
