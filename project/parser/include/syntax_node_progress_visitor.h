#pragma once

#include "i_syntax_node_visitor.h"

#include "nonterminals/function_statment_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include <functional>
#include <memory>
// #include "utils.h"
#include "progress_counter.h"

template< typename State >
class SyntaxNodeProgressVisitor : public ISyntaxNodeVisitor
{
public:
   class Handlers
   {
   public: 
      enum class Impact
      {
          MOVE,
          NO_MOVE,
          ERROR
      };
      Handlers( size_t states_number, const State& inital_state )
          : mState{ inital_state }
          , mProgressCounter{ states_number }
      {

      }
      struct HandlerReturn
      {
          State state;
          Impact impact;
      };
      std::function< HandlerReturn ( const State& state, const ISyntaxNodeSP& ) > default_handler = []( const State& state, const ISyntaxNodeSP& )->HandlerReturn { return { state, Impact::ERROR }; };
      std::function< HandlerReturn( const State& state, const BolSyntaxNodeSP& ) > bol_syntax_node = [ this ] ( const State& state,  const BolSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const PlusSyntaxNodeSP& ) > plus_syntax_node = [ this ] ( const State& state,  const PlusSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const MinusSyntaxNodeSP& ) > minus_syntax_node = [ this ] ( const State& state,  const MinusSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const AsteriskSyntaxNodeSP& ) > asterisk_syntax_node = [ this ] ( const State& state,  const AsteriskSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const VarSyntaxNodeSP& ) > var_syntax_node = [ this ] ( const State& state,  const VarSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const SlashSyntaxNodeSP& ) > slash_syntax_node = [ this ] ( const State& state,  const SlashSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const DoubleSyntaxNodeSP& ) > double_syntax_node = [ this ] ( const State& state,  const DoubleSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const IntSyntaxNodeSP& ) > int_syntax_node = [ this ] ( const State& state,  const IntSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const FSyntaxNodeSP& ) > f_syntax_node = [ this ] ( const State& state,  const FSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const UnExprSyntaxNodeSP& ) > un_expr_syntax_node = [ this ] ( const State& state,  const UnExprSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const BinExprSyntaxNodeSP& ) > bin_expr_syntax_node = [ this ] ( const State& state,  const BinExprSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const MemberExpressionSyntaxNodeSP& ) > member_expression_syntax_node = [ this ] ( const State& state,  const MemberExpressionSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const EolSyntaxNodeSP& ) > eol_syntax_node = [ this ] ( const State& state,  const EolSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const SemicolonSyntaxNodeSP& ) > semicolon_syntax_node = [ this ] ( const State& state,  const SemicolonSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const StatmentSyntaxNodeSP& ) > statment_syntax_node = [ this ] ( const State& state,  const StatmentSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ColonSyntaxNodeSP& ) > colon_syntax_node = [ this ] ( const State& state,  const ColonSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ScopeSyntaxNodeSP& ) > scope_statment_syntax_node = [ this ] ( const State& state,  const ScopeSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const FunctionScopeSyntaxNodeSP& ) > function_scope_statment_syntax_node = [ this ] ( const State& state,  const FunctionScopeSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ArraySyntaxNodeSP& ) > array_syntax_node = [ this ] ( const State& state,  const ArraySyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ObjectSyntaxNodeSP& ) > object_syntax_node = [ this ] ( const State& state,  const ObjectSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ObjectPairSyntaxNodeSP& ) > object_pair_syntax_node = [ this ] ( const State& state,  const ObjectPairSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const OpenCurlyBracketSyntaxNodeSP& ) > open_curly_bracket_syntax_node = [ this ] ( const State& state,  const OpenCurlyBracketSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const CloseCurlyBracketSyntaxNodeSP& ) > close_curly_bracket_syntax_node = [ this ] ( const State& state,  const CloseCurlyBracketSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const OpenCircleBracketSyntaxNodeSP& ) > open_circle_bracket_syntax_node = [ this ] ( const State& state,  const OpenCircleBracketSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const CloseCircleBracketSyntaxNodeSP& ) > close_circle_bracket_syntax_node = [ this ] ( const State& state,  const CloseCircleBracketSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const OpenSquareBracketSyntaxNodeSP& ) > open_square_bracket_syntax_node = [ this ] ( const State& state,  const OpenSquareBracketSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const CloseSquareBracketSyntaxNodeSP& ) > close_square_bracket_syntax_node = [ this ] ( const State& state,  const CloseSquareBracketSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const VaribleSyntaxNodeSP& ) > varible_syntax_node = [ this ] ( const State& state,  const VaribleSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const PrintStatmentSyntaxNodeSP& ) > print_statment_syntax_node = [ this ] ( const State& state,  const PrintStatmentSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const VaribleAssigmentStatmentSyntaxNodeSP& ) > varible_assigment_statment_syntax_node =
         [ this ] ( const State& state,  const VaribleAssigmentStatmentSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const NameSyntaxNodeSP& ) > name_syntax_node = [ this ] ( const State& state,  const NameSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const StringSyntaxNodeSP& ) > string_syntax_node = [ this ] ( const State& state,  const StringSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const CommaSyntaxNodeSP& ) > comma_syntax_node = [ this ] ( const State& state,  const CommaSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const PrintSyntaxNodeSP& ) > print_syntax_node = [ this ] ( const State& state,  const PrintSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const EqualSyntaxNodeSP& ) > equal_syntax_node = [ this ] ( const State& state,  const EqualSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const NotSyntaxNodeSP& ) > not_syntax_node = [ this ] ( const State& state,  const NotSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const LessSyntaxNodeSP& ) > less_syntax_node = [ this ] ( const State& state,  const LessSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const MoreSyntaxNodeSP& ) > more_syntax_node = [ this ] ( const State& state,  const MoreSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const IfSyntaxNodeSP& ) > if_syntax_node = [ this ] ( const State& state,  const IfSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ElseSyntaxNodeSP& ) > else_syntax_node = [ this ] ( const State& state,  const ElseSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const IfStatmentSyntaxNodeSP& ) > if_statment_syntax_node = [ this ] ( const State& state,  const IfStatmentSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const WhileSyntaxNodeSP& ) > while_syntax_node = [ this ] ( const State& state,  const WhileSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const WhileStatmentSyntaxNodeSP& ) > while_statment_syntax_node = [ this ] ( const State& state,  const WhileStatmentSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const GotoSyntaxNodeSP& ) > goto_syntax_node = [ this ] ( const State& state,  const GotoSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const FunctionStatmentSyntaxNodeSP& ) > function_statment_syntax_node = [ this ] ( const State& state,  const FunctionStatmentSyntaxNodeSP& node )
      { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const FunctionSyntaxNodeSP& ) > function_syntax_node = [ this ] ( const State& state,  const FunctionSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const FunctionCallSyntaxNodeSP& ) > function_call_syntax_node = [ this ] ( const State& state,  const FunctionCallSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ReturnSyntaxNodeSP& ) > return_syntax_node = [ this ] ( const State& state,  const ReturnSyntaxNodeSP& node ) { return default_handler( state, node ); };
      std::function< HandlerReturn( const State& state, const ReturnStatmentSyntaxNodeSP& ) > return_statment_syntax_node = [ this ] ( const State& state,  const ReturnStatmentSyntaxNodeSP& node )
      { return default_handler( state, node ); };

      template<typename Node>
      void call( const auto Handlers::* callback, const State& state, const Node& node )
      {
          const auto& handler_return = (this->*callback)( state, node );
          mState = handler_return.state;
          if( handler_return.impact == Impact::MOVE )
          {
              mProgressCounter.Step();
          }
          else if( handler_return.impact == Impact::NO_MOVE )
          {
          }
          else
          {
              mProgressCounter.Reset();
          }
      }
      double readiness() const
      {
         return mProgressCounter.Status();
      }
      State state() const
      {
         return mState;
      }
   private:
      State mState;
      ProgressCounter mProgressCounter;
   };

   SyntaxNodeProgressVisitor( const Handlers& handlers )
       : mHandlers{ handlers }
   {
   }

   double readiness() const
   {
       return mHandlers.readiness();
   }

   State state() const
   {
       return mHandlers.state();
   }
   ~SyntaxNodeProgressVisitor() = default;

   void visit( const BolSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::bol_syntax_node, mHandlers.state(), node );
   }

   void visit( const EolSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::eol_syntax_node, mHandlers.state(), node );
   }

   void visit( const PlusSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::plus_syntax_node, mHandlers.state(), node );
   }

   void visit( const MinusSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::minus_syntax_node, mHandlers.state(), node );
   }

   void visit( const AsteriskSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::asterisk_syntax_node, mHandlers.state(), node );
   }
   
   void visit( const VarSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::var_syntax_node, mHandlers.state(), node );
   }
   
   void visit( const SlashSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::slash_syntax_node, mHandlers.state(), node );
   }

   void visit( const DoubleSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::double_syntax_node, mHandlers.state(), node );
   }
   
   void visit( const IntSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::int_syntax_node, mHandlers.state(), node );
   }

   void visit( const FSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::f_syntax_node, mHandlers.state(), node );
   }

   void visit( const UnExprSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::un_expr_syntax_node, mHandlers.state(), node );
   }
  
   void visit( const BinExprSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::bin_expr_syntax_node, mHandlers.state(), node );
   }
  
   void visit( const MemberExpressionSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::member_expression_syntax_node, mHandlers.state(), node );
   }
   
   void visit( const SemicolonSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::semicolon_syntax_node, mHandlers.state(), node );
   }
   
   void visit( const ColonSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::colon_syntax_node, mHandlers.state(), node );
   }

   void visit( const StatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const ScopeSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::scope_statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const FunctionScopeSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::function_scope_statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const ArraySyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::array_syntax_node, mHandlers.state(), node );
   }

   void visit( const ObjectSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::object_syntax_node, mHandlers.state(), node );
   }

   void visit( const ObjectPairSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::object_pair_syntax_node, mHandlers.state(), node );
   }

   void visit( const OpenCurlyBracketSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::open_curly_bracket_syntax_node, mHandlers.state(), node );
   }

   void visit( const CloseCurlyBracketSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::close_curly_bracket_syntax_node, mHandlers.state(), node );
   }

   void visit( const OpenCircleBracketSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::open_circle_bracket_syntax_node, mHandlers.state(), node );
   }

   void visit( const CloseCircleBracketSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::close_circle_bracket_syntax_node, mHandlers.state(), node );
   }

   void visit( const OpenSquareBracketSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::open_square_bracket_syntax_node, mHandlers.state(), node );
   }

   void visit( const CloseSquareBracketSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::close_square_bracket_syntax_node, mHandlers.state(), node );
   }

   void visit( const VaribleSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::varible_syntax_node, mHandlers.state(), node );
   }

   void visit( const PrintStatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::print_statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const VaribleAssigmentStatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::varible_assigment_statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const NameSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::name_syntax_node, mHandlers.state(), node );
   }

   void visit( const StringSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::string_syntax_node, mHandlers.state(), node );
   }

   void visit( const CommaSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::comma_syntax_node, mHandlers.state(), node );
   }

   void visit( const PrintSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::print_syntax_node, mHandlers.state(), node );
   }

   void visit( const EqualSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::equal_syntax_node, mHandlers.state(), node );
   }

   void visit( const NotSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::not_syntax_node, mHandlers.state(), node );
   }

   void visit( const LessSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::less_syntax_node, mHandlers.state(), node );
   }

   void visit( const MoreSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::more_syntax_node, mHandlers.state(), node );
   }

   void visit( const IfSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::if_syntax_node, mHandlers.state(), node );
   }

   void visit( const ElseSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::else_syntax_node, mHandlers.state(), node );
   }

   void visit( const IfStatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::if_statment_syntax_node, mHandlers.state(), node );
   }
   void visit( const WhileSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::while_syntax_node, mHandlers.state(), node );
   }

   void visit( const WhileStatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::while_statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const GotoSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::goto_syntax_node, mHandlers.state(), node );
   }

   void visit( const FunctionStatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::function_statment_syntax_node, mHandlers.state(), node );
   }

   void visit( const FunctionSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::function_syntax_node, mHandlers.state(), node );
   }

   void visit( const FunctionCallSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::function_call_syntax_node, mHandlers.state(), node );
   }

   void visit( const ReturnSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::return_syntax_node, mHandlers.state(), node );
   }

   void visit( const ReturnStatmentSyntaxNodeSP& node ) override
   {
      mHandlers.call( &Handlers::return_statment_syntax_node, mHandlers.state(), node );
   }

 private:
   Handlers mHandlers;
};
template<typename State>
using SyntaxNodeProgressVisitorSP = std::shared_ptr< SyntaxNodeProgressVisitor< State > >;
