#include "grammars/function_call_grammar.h"
#include "nonterminals/array_syntax_node.h"
#include "nonterminals/function_statment_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "nonterminals/statment_syntax_node.h"
#include "nonterminals/function_call_syntax_node.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include "nonterminals/varible_syntax_node.h"
#include "terminals/asterisk_syntax_node.h"
#include "terminals/close_circle_bracket_syntax_node.h"
#include "terminals/close_square_bracket_syntax_node.h"
#include "terminals/minus_syntax_node.h"
#include "terminals/name_syntax_node.h"
#include "terminals/comma_syntax_node.h"
#include "terminals/semicolon_syntax_node.h"
#include "terminals/slash_syntax_node.h"
#include "utils.h"

FunctionCall::FunctionCall()
{
   enum class State
   {
      START,
      FINISH,
      ERROR,
      NAME,
      MEMBER_EXPRESSION,
      ARGUMENT,
      OPEN_CIRCLE_BRACKET,
      CLOSE_CIRCLE_BRACKET,
      COMMA,
      FUNCTION_CALL,
      SEMICOLON,
   };

   // NAME|MEMBER_EXPRESSION OPEN_CIRCLE_BRACKET (NAME|F|BIN_EXPR|UN_EXPR|FUNCTION_CALL COMMA?)+ CLOSE_CIRCLE_BRACKET
   mProductions.emplace_back(
      []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
      {
         auto &s = stack;
         (void) s;
         bool is_open_circle_bracket_found = false;
         bool is_close_circle_bracket_found = false;
         size_t distance_between_open_close_circle_bracket = 0;
         
         auto it = stack.rbegin();
         SyntaxNodeEmptyVisitor::Handlers open_circle_bracket_handler;
         for( ; it != stack.rend(); ++it )
         {
            open_circle_bracket_handler.open_circle_bracket_syntax_node = [ &is_open_circle_bracket_found ]( const OpenCircleBracketSyntaxNodeSP& /* node */ )
            { is_open_circle_bracket_found = true; };
            const auto& open_circle_bracket_visitor = std::make_shared< SyntaxNodeEmptyVisitor >( open_circle_bracket_handler );
            ( *it )->accept( open_circle_bracket_visitor );
            if( is_open_circle_bracket_found )
            {
               distance_between_open_close_circle_bracket = std::distance( stack.rbegin(), it ) + 1;
               break;
            }
         }
         
         if( !is_open_circle_bracket_found )
             return Progress{ .readiness = 0.0 };
         
         SyntaxNodeEmptyVisitor::Handlers close_circle_bracket_handler;
         for( auto it1 = stack.rbegin(); it1 != it; ++it1 )
         {
            close_circle_bracket_handler.close_circle_bracket_syntax_node = [ &is_close_circle_bracket_found ]( const CloseCircleBracketSyntaxNodeSP& /* node */ )
            { is_close_circle_bracket_found = true; };
            const auto& close_circle_bracket_visitor = std::make_shared< SyntaxNodeEmptyVisitor >( close_circle_bracket_handler );
            ( *it1 )->accept( close_circle_bracket_visitor );
            if( is_close_circle_bracket_found )
            {
               break;
            }
         }

         const size_t minimal_size = distance_between_open_close_circle_bracket + 1;
         size_t minimal_steps_number = 0;
         if( is_open_circle_bracket_found && !is_close_circle_bracket_found )
             minimal_steps_number = minimal_size + 1;
         return find_grammar_matching_progress(stack, minimal_size, [&stack, &lookahead, &minimal_steps_number]( size_t n ) -> PlanOrProgress
         {
             std::variant< VaribleSyntaxNodeSP, MemberExpressionSyntaxNodeSP > function_as_name_or_member_expression;
             OpenCircleBracketSyntaxNodeSP open_circle_bracket;
             std::vector< ISyntaxNodeSP > arguments;
             std::vector< ISyntaxNodeSP > commas;
             CloseCircleBracketSyntaxNodeSP close_circle_bracket;
             
             const Stack& s = stack;
             SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
             using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
             using HandlerReturn = Handlers::HandlerReturn;
             using Impact = Handlers::Impact;

             handlers.default_handler = []( const State& state, const ISyntaxNodeSP& ) -> HandlerReturn
             { 
                return { State::ERROR, Impact::ERROR };
             };
             handlers.name_syntax_node = [ &function_as_name_or_member_expression, &arguments ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::START )
                {
                    // function_as_name_or_member_expression = node;
                    function_as_name_or_member_expression = std::make_shared< VaribleSyntaxNode >( node, node->lexical_tokens() );
                    return { State::NAME, Impact::MOVE };
                }
                else if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( std::make_shared<VaribleSyntaxNode>(node, node->lexical_tokens()) );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.f_syntax_node = [ &arguments ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.bin_expr_syntax_node = [ &arguments ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.un_expr_syntax_node = [ &arguments ]( const State& state, const UnExprSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.varible_assigment_statment_syntax_node = [ &arguments ]( const State& state, const VaribleAssigmentStatmentSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.function_statment_syntax_node = [ &arguments ]( const State& state, const FunctionStatmentSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.function_call_syntax_node = [ &arguments ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.array_syntax_node = [ &arguments ]( const State& state, const ArraySyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.object_syntax_node = [ &arguments ]( const State& state, const ObjectSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                    arguments.emplace_back( node );
                    return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.member_expression_syntax_node = [ &arguments, &function_as_name_or_member_expression ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::START )
                {
                    function_as_name_or_member_expression = node;
                    return { State::MEMBER_EXPRESSION, Impact::MOVE };
                }
                else if( state == State::OPEN_CIRCLE_BRACKET || state == State::COMMA )
                {
                   arguments.push_back( node );
                   return { State::ARGUMENT, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.comma_syntax_node = [ &commas ]( const State& state, const CommaSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::ARGUMENT )
                {
                    commas.emplace_back( node );
                    return { State::COMMA, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.open_circle_bracket_syntax_node = [ &open_circle_bracket ]( const State& state, const OpenCircleBracketSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::NAME || state == State::MEMBER_EXPRESSION )
                {
                    open_circle_bracket = node;
                    return { State::OPEN_CIRCLE_BRACKET, Impact::MOVE };
                }
                return { state, Impact::ERROR };
             };
             handlers.close_circle_bracket_syntax_node = [ &close_circle_bracket, &lookahead ]( const State& state, const CloseCircleBracketSyntaxNodeSP& node ) -> HandlerReturn
             {
                if( state == State::ARGUMENT || state == State::OPEN_CIRCLE_BRACKET )
                {
                  if( lookahead && ( check_type<SemicolonSyntaxNode>( lookahead ) || 
                                     check_type<CloseCircleBracketSyntaxNode>( lookahead ) ||
                                     check_type<CloseCurlyBracketSyntaxNode>( lookahead ) ||
                                     check_type<CloseSquareBracketSyntaxNode>( lookahead ) ||
                                     check_type<PlusSyntaxNode>( lookahead ) ||
                                     check_type<MinusSyntaxNode>( lookahead ) ||
                                     check_type<AsteriskSyntaxNode>( lookahead ) ||
                                     check_type<SlashSyntaxNode>( lookahead ) ||
                                     check_type<CommaSyntaxNode>( lookahead ) ) )
                  {
                      close_circle_bracket = node;
                      return { State::FINISH, Impact::MOVE };
                  }
                }
                return { state, Impact::ERROR };
             };
             auto iteration_result = iterate_over_last_n_nodesv2< State >( stack, n, handlers );

             PlanOrProgress plan_or_progress;
             if( iteration_result.state == State::ERROR )
             {
                 plan_or_progress = Progress{ .readiness = 0 };
             }  
             else if( iteration_result.state == State::FINISH )
             {
                 Plan plan;
                 std::visit([&plan](auto&& arg)
                 {
                     using T = std::decay_t<decltype(arg)>;
                     if constexpr (std::is_same_v<T, VaribleSyntaxNodeSP>)
                         plan.to_remove.nodes.push_back( arg );
                     else if constexpr (std::is_same_v<T, MemberExpressionSyntaxNodeSP>)
                         plan.to_remove.nodes.push_back( arg );
                 }, function_as_name_or_member_expression );
                 plan.to_remove.nodes.push_back( open_circle_bracket );
                 for( const auto& argument : arguments )
                    plan.to_remove.nodes.push_back( argument );
                 for( const auto& comma : commas )
                    plan.to_remove.nodes.push_back( comma );
                 plan.to_remove.nodes.push_back( close_circle_bracket );

                 std::visit([&plan, &arguments](auto&& arg)
                 {
                     using T = std::decay_t<decltype(arg)>;
                     if constexpr (std::is_same_v<T, VaribleSyntaxNodeSP>)
                     {
                         const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( arg, arguments );
                         plan.to_add.nodes.push_back( function_call );
                     }
                     else if constexpr (std::is_same_v<T, MemberExpressionSyntaxNodeSP>)
                     {
                         const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( arg, arguments );
                         plan.to_add.nodes.push_back( function_call );
                     }
                 }, function_as_name_or_member_expression );
                 plan_or_progress = plan;
             }
             else {
                 plan_or_progress = Progress{ .readiness = iteration_result.readiness };
             }
             return plan_or_progress;
         });

      } );
   
   // FUNCTION_CALL SEMICOLON
   mProductions.emplace_back(
      []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
      {
         const size_t minimal_size = 2;
         size_t minimal_steps_number = 2;
         return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number, &lookahead]( size_t n )->PlanOrProgress
                                                        
         {
           FunctionCallSyntaxNodeSP function_call;
           SemicolonSyntaxNodeSP semicolon;

           const Stack& s = stack;
           SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
           using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
           using HandlerReturn = Handlers::HandlerReturn;
           using Impact = Handlers::Impact;

           handlers.default_handler = []( const State& state, const ISyntaxNodeSP& ) -> HandlerReturn
           { 
              return { State::ERROR, Impact::ERROR };
           };
           handlers.function_call_syntax_node = [ &function_call ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
           {
              if( state == State::START )
              {
                 function_call = node;
                 return { State::FUNCTION_CALL, Impact::MOVE };
              }
              return { state, Impact::ERROR };
           };
           handlers.semicolon_syntax_node = [ &semicolon ]( const State& state, const SemicolonSyntaxNodeSP& node ) -> HandlerReturn
           {
              if( state == State::FUNCTION_CALL )
              {
                 semicolon = node;
                 return { State::FINISH, Impact::MOVE };
              }
              return { state, Impact::ERROR };
           };
           auto iteration_result = iterate_over_last_n_nodesv2< State >( stack, n, handlers );

           PlanOrProgress plan_or_progress;
           if( iteration_result.state == State::ERROR )
           {
               plan_or_progress = Progress{ .readiness = 0 };
           }  
           else if( iteration_result.state == State::FINISH )
           {
               Plan plan;
               plan.to_remove.nodes.push_back( function_call );
               plan.to_remove.nodes.push_back( semicolon );

               plan.to_add.nodes.push_back( function_call );
               plan_or_progress = plan;
           }
           else
           {
               plan_or_progress = Progress{ .readiness = iteration_result.readiness };
           }
           return plan_or_progress;
         });
      } );
}
