#include "nonterminals/function_statment_syntax_node.h"

#include <string>
#include "i_syntax_node_visitor.h"
#include "syntax_node_empty_visitor.h"
#include "nonterminals/scope_statment_syntax_node.h"
#include "nonterminals/function_scope_statment_syntax_node.h"
#include "terminals/name_syntax_node.h"
#include "utils.h"

FunctionStatmentSyntaxNode::FunctionStatmentSyntaxNode( const FunctionStatmentSyntaxNode& function_statment_syntax_node )
   : ISyntaxNode{ Token_Type::FUNCTION_STATMENT }
   , mName{ function_statment_syntax_node.name() }
{
   mTokens = function_statment_syntax_node.lexical_tokens();
}

// FunctionStatmentSyntaxNode::FunctionStatmentSyntaxNode( const std::string& name )
//    : ISyntaxNode{ Token_Type::FUNCTION_STATMENT }
//    , mName{ name }
// {
// }

// FunctionStatmentSyntaxNode::FunctionStatmentSyntaxNode( const std::string& name, const std::vector< ISyntaxNodeSP >& arguments, const ScopeSyntaxNodeSP& scope )
//    : ISyntaxNode{ Token_Type::FUNCTION_STATMENT }
//    , mName{ name }
// {
//    for( const auto& argument : arguments )
//    {
//       ISyntaxNodeSP child = argument;
//
//       // if( check_type< NameSyntaxNode >( argument ) )
//       // {
//          // const auto& name_node = std::dynamic_pointer_cast< NameSyntaxNode >( argument );
//          // child = std::make_shared< VaribleSyntaxNode >( name_node->value() );
//       // }
//
//       add_back( child );
//    }
//    add_back( scope );
// }

FunctionStatmentSyntaxNode::FunctionStatmentSyntaxNode( const NameSyntaxNodeSP& name_syntax_node, const std::vector< ISyntaxNodeSP >& arguments, const ScopeSyntaxNodeSP& scope, const std::vector< LexicalTokens::LexicalToken >& lexical_tokens )
   : ISyntaxNode{ Token_Type::FUNCTION_STATMENT }
   , mName{ name_syntax_node->value() }
{
   mTokens = lexical_tokens;
  add_back( name_syntax_node );
   for( const auto& argument : arguments )
   {
      ISyntaxNodeSP child = argument;

      if( !check_type< NameSyntaxNode >( argument ) )
      {
         child = std::make_shared< NameSyntaxNode >( argument->lexical_tokens()[0] );
      }

      add_back( child );
   }
   add_back( scope );
}

void FunctionStatmentSyntaxNode::accept( const ISyntaxNodeVisitorSP& visitor )
{
   visitor->visit( shared_from_this() );
}

bool FunctionStatmentSyntaxNode::compare( const ISyntaxNode& node ) const
{
   bool is_equal = false;
   SyntaxNodeEmptyVisitor::Handlers handlers;
   handlers.function_statment_syntax_node = [ this, &is_equal ]( const FunctionStatmentSyntaxNodeSP& node )
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
      if( node->name() != this->name() )
        return;
      is_equal = true;
   };
   const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
   const_cast< ISyntaxNode& >( node ).accept( visitor );

   return is_equal;
}

std::string FunctionStatmentSyntaxNode::name() const
{
   return mName;
   // return std::dynamic_pointer_cast< NameSyntaxNode >( this->operator[]( 0 ) );
}

FunctionScopeSyntaxNodeSP FunctionStatmentSyntaxNode::scope() const
{
   return std::dynamic_pointer_cast< FunctionScopeSyntaxNode >( this->operator[]( this->Children().size() - 1 ) );
}

std::vector< NameSyntaxNodeSP > FunctionStatmentSyntaxNode::arguments() const
{
   std::vector< NameSyntaxNodeSP > result;
   for( auto index = 1; index < mChildren.size(); ++index )
   {
     const auto& name = std::dynamic_pointer_cast< NameSyntaxNode >( this->operator[]( index ) );
     result.emplace_back( name );
   }
   return result;
}

std::vector< LexicalTokens::LexicalToken > FunctionStatmentSyntaxNode::lexical_tokens() const
{ 
    return mTokens; 
}
