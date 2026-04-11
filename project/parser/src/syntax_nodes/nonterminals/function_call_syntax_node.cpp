#include "nonterminals/function_call_syntax_node.h"

#include "i_syntax_node_visitor.h"
#include "is_last_nodes.h"
#include "syntax_node_empty_visitor.h"
#include "terminals/name_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"

FunctionCallSyntaxNode::FunctionCallSyntaxNode( const FunctionCallSyntaxNode& function_call_syntax_node )
   : ISyntaxNode{ Token_Type::FUNCTION_CALL }
   // , mName{ function_call_syntax_node.name() }
   // , mSignature{ function_call_syntax_node.mSignature }
{
   mTokens = function_call_syntax_node.lexical_tokens();
}

// FunctionCallSyntaxNode::FunctionCallSyntaxNode( const std::string& name )
//    : ISyntaxNode{ Token_Type::FUNCTION_CALL }
//    , mName{ name }
// {
// }

// FunctionCallSyntaxNode::FunctionCallSyntaxNode( const std::string& name, const std::vector< ISyntaxNodeSP >& arguments )
//    : ISyntaxNode{ Token_Type::FUNCTION_CALL }
//    , mName{ name }
//    // , mSignature{ mName, arguments.size() }
// {
//    for( const auto& argument : arguments )
//    {
//       add_back( argument );
//    }
// }

FunctionCallSyntaxNode::FunctionCallSyntaxNode( const VaribleSyntaxNodeSP& varible_syntax_node, const std::vector< ISyntaxNodeSP >& arguments_syntax_nodes )
   : ISyntaxNode{ Token_Type::FUNCTION_CALL }
   // , mType{ FunctionCallSyntaxNode::Type::BY_NAME }
   // , mName{ name_syntax_node->value() }
   // , mSignature{ mName, arguments_syntax_nodes.size() }
{
   for( const auto& argument : arguments_syntax_nodes )
   {
      add_back( argument );
   }
   add_back( varible_syntax_node );
}

FunctionCallSyntaxNode::FunctionCallSyntaxNode( const MemberExpressionSyntaxNodeSP& member_expression_syntax_node, const std::vector< ISyntaxNodeSP >& arguments_syntax_nodes )
   : ISyntaxNode{ Token_Type::FUNCTION_CALL }
   // , mType{ FunctionCallSyntaxNode::Type::BY_OPERATION_EXPRESSION }
   // , mName{ name_syntax_node->value() }
   // , mSignature{ mName, arguments_syntax_nodes.size() }
{
   for( const auto& argument : arguments_syntax_nodes )
   {
      add_back( argument );
   }
   add_back( member_expression_syntax_node );
}

void FunctionCallSyntaxNode::accept( const ISyntaxNodeVisitorSP& visitor )
{
   visitor->visit( shared_from_this() );
}

bool FunctionCallSyntaxNode::compare( const ISyntaxNode& node ) const
{
   bool is_equal = false;
   SyntaxNodeEmptyVisitor::Handlers handlers;
   handlers.function_call_syntax_node = [ this, &is_equal ]( const FunctionCallSyntaxNodeSP& node )
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
      // if( node->name() != this->name() )
      //   return;
      is_equal = true;
   };
   const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
   const_cast< ISyntaxNode& >( node ).accept( visitor );

   return is_equal;
}

// FunctionCallSyntaxNode::Type FunctionCallSyntaxNode::type() const
// {
//     return mType;
// }
//
// std::string FunctionCallSyntaxNode::name() const
// {
//    return mName;
// }
//
std::vector< ISyntaxNodeSP > FunctionCallSyntaxNode::arguments() const
{
    std::vector< ISyntaxNodeSP > result;
    for( int i=1; i < mChildren.size(); ++i)
      result.emplace_back( this->operator[](i) );
    return result; 
}

std::vector< LexicalTokens::LexicalToken > FunctionCallSyntaxNode::lexical_tokens() const
{ 
    return { mTokens }; 
};
