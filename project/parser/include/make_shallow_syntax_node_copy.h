#pragma once

#include "i_syntax_node.h"
#include "i_syntax_node_visitor.h"

#include "nonterminals/function_statment_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include <memory>

namespace
{
class SyntaxNodeShallowCopyVisitor : public ISyntaxNodeVisitor
{
public:
   ~SyntaxNodeShallowCopyVisitor() = default;

   void visit( const BolSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< BolSyntaxNode >( *node );
   }

   void visit( const EolSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< EolSyntaxNode >( *node );
   }

   void visit( const PlusSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< PlusSyntaxNode >( *node );
   }

   void visit( const MinusSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< MinusSyntaxNode >( *node );
   }

   void visit( const AsteriskSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< AsteriskSyntaxNode >( *node );
   }
   
   void visit( const VarSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< VarSyntaxNode >( *node );
   }
   
   void visit( const SlashSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< SlashSyntaxNode >( *node );
   }

   void visit( const DoubleSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< DoubleSyntaxNode >( *node );
   }

   void visit( const IntSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< IntSyntaxNode >( *node );
   }

   void visit( const FSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< FSyntaxNode >( *node );
   }

   void visit( const UnExprSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< UnExprSyntaxNode >( *node );
   }
   
   void visit( const BinExprSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< BinExprSyntaxNode >( *node );
   }
   
   void visit( const MemberExpressionSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< MemberExpressionSyntaxNode >( *node );
   }

   void visit( const SemicolonSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< SemicolonSyntaxNode >( *node );
   }
   
   void visit( const ColonSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ColonSyntaxNode >( *node );
   }

   void visit( const StatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< StatmentSyntaxNode >( *node );
   }

   void visit( const ScopeSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ScopeSyntaxNode >( *node );
   }
   
   void visit( const FunctionScopeSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< FunctionScopeSyntaxNode >( *node );
   }
   
   void visit( const ArraySyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ArraySyntaxNode >( *node );
   }

   void visit( const ObjectSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ObjectSyntaxNode >( *node );
   }

   void visit( const ObjectPairSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ObjectPairSyntaxNode >( *node );
   }

   void visit( const OpenCurlyBracketSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< OpenCurlyBracketSyntaxNode >( *node );
   }

   void visit( const CloseCurlyBracketSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< CloseCurlyBracketSyntaxNode >( *node );
   }

   void visit( const OpenCircleBracketSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< OpenCircleBracketSyntaxNode >( *node );
   }

   void visit( const CloseCircleBracketSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< CloseCircleBracketSyntaxNode >( *node );
   }
   
   void visit( const OpenSquareBracketSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< OpenSquareBracketSyntaxNode >( *node );
   }

   void visit( const CloseSquareBracketSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< CloseSquareBracketSyntaxNode >( *node );
   }

   void visit( const VaribleSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< VaribleSyntaxNode >( *node );
   }

   void visit( const PrintStatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< PrintStatmentSyntaxNode >( *node );
   }

   void visit( const VaribleAssigmentStatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( *node );
   }

   void visit( const NameSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< NameSyntaxNode >( *node );
   }
   
   void visit( const StringSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< StringSyntaxNode >( *node );
   }

   void visit( const CommaSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< CommaSyntaxNode >( *node );
   }

   void visit( const PrintSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< PrintSyntaxNode >( *node );
   }

   void visit( const EqualSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< EqualSyntaxNode >( *node );
   }

   void visit( const NotSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< NotSyntaxNode >( *node );
   }

   void visit( const LessSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< LessSyntaxNode >( *node );
   }

   void visit( const MoreSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< MoreSyntaxNode >( *node );
   }

   void visit( const IfSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< IfSyntaxNode >( *node );
   }
   
   void visit( const ElseSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ElseSyntaxNode >( *node );
   }

   void visit( const IfStatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< IfStatmentSyntaxNode >( *node );
   }
   void visit( const WhileSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< WhileSyntaxNode >( *node );
   }

   void visit( const WhileStatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< WhileStatmentSyntaxNode >( *node );
   }

   void visit( const GotoSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< GotoSyntaxNode >( *node );
   }

   void visit( const FunctionStatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< FunctionStatmentSyntaxNode >( *node );
   }

   void visit( const FunctionSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< FunctionSyntaxNode >( *node );
   }

   void visit( const FunctionCallSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< FunctionCallSyntaxNode >( *node );
   }

   void visit( const ReturnSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ReturnSyntaxNode >( *node );
   }

   void visit( const ReturnStatmentSyntaxNodeSP& node ) override
   {
      mCopy = std::make_shared< ReturnStatmentSyntaxNode >( *node );
   }

   ISyntaxNodeSP get_copy() const
   {
      return mCopy;
   }

private:
   ISyntaxNodeSP mCopy;
};
using SyntaxNodeShallowCopyVisitorSP = std::shared_ptr< SyntaxNodeShallowCopyVisitor >;
} // namespace

static ISyntaxNodeSP make_shallow_copy( const ISyntaxNodeSP& node )
{
   const auto& visitor = std::make_shared< SyntaxNodeShallowCopyVisitor >();
   node->accept( visitor );
   return visitor->get_copy();
};
