#include "scope_grammar.h"

#include "nonterminals/if_statment_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "nonterminals/return_statment_syntax_node.h"
#include "nonterminals/statment_syntax_node.h"
#include "i_grammar.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include "syntax_node_empty_visitor.h"
#include "utils.h"

#include <vector>

Scope::Scope()
{
  enum class State
  {
     START,
     FINISH,
     ERROR,
     BOL,
     EOL,
     OPEN_CURLY_BRACKET,
     STATMENT,
     CLOSE_CURLY_BRACKET,
     SCOPE_STATMENT
  };

  // OPEN_CURLY_BRACKET STATMENT+ CLOSE_CURLY_BRACKET
  mProductions.emplace_back(
     [ /* this */ ]( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        bool is_open_curly_bracket_found = false;
        bool is_close_curly_bracket_found = false;
        size_t distance_between_open_close_curly_bracket = 0;

        SyntaxNodeEmptyVisitor::Handlers close_curly_bracket_handler;
        close_curly_bracket_handler.close_curly_bracket_syntax_node = [ &is_close_curly_bracket_found ]( const CloseCurlyBracketSyntaxNodeSP& /* node */ )
        { is_close_curly_bracket_found = true; };
        const auto& close_curly_bracket_visitor = std::make_shared< SyntaxNodeEmptyVisitor >( close_curly_bracket_handler );
        ( *stack.rbegin() )->accept( close_curly_bracket_visitor );

        SyntaxNodeEmptyVisitor::Handlers open_curly_bracket_handler;
        for( auto it = stack.rbegin(); it != stack.rend(); ++it )
        {
           open_curly_bracket_handler.open_curly_bracket_syntax_node = [ &is_open_curly_bracket_found ]( const OpenCurlyBracketSyntaxNodeSP& /* node */ )
           { is_open_curly_bracket_found = true; };
           const auto& open_curly_bracket_visitor = std::make_shared< SyntaxNodeEmptyVisitor >( open_curly_bracket_handler );
           ( *it )->accept( open_curly_bracket_visitor );
           if( is_open_curly_bracket_found )
           {
              distance_between_open_close_curly_bracket = std::distance( stack.rbegin(), it ) + 1;
              break;
           }
        }

        if( !is_open_curly_bracket_found /*|| !is_close_curly_bracket_found*/ )
            return Progress{ .readiness = 0.0 };

        const size_t minimal_size = distance_between_open_close_curly_bracket;
        size_t minimal_steps_number = 0;
        if( is_open_curly_bracket_found && !is_close_curly_bracket_found )
            minimal_steps_number = minimal_size + 1;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number]( size_t n )->PlanOrProgress
                                                        {
                                                        OpenCurlyBracketSyntaxNodeSP open_curly_bracket;
                                                        std::vector< ISyntaxNodeSP > expressions;
                                                        CloseCurlyBracketSyntaxNodeSP close_curly_bracket;

                                                        const Stack& s = stack;
                                                        SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
                                                        using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
                                                        using HandlerReturn = Handlers::HandlerReturn;
                                                        using Impact = Handlers::Impact;

                                                         handlers.default_handler = []( const State& state, const ISyntaxNodeSP& ) -> HandlerReturn
                                                         { 
                                                            return { State::ERROR, Impact::ERROR };
                                                         };

                                                        handlers.open_curly_bracket_syntax_node = [ &open_curly_bracket ]( const State& state, const OpenCurlyBracketSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              open_curly_bracket = node;
                                                              return { State::OPEN_CURLY_BRACKET, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.statment_syntax_node = [ &expressions ]( const State& state, const StatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.if_statment_syntax_node = [ &expressions ]( const State& state, const IfStatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.while_statment_syntax_node = [ &expressions ]( const State& state, const WhileStatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.return_statment_syntax_node = [ &expressions ]( const State& state, const ReturnStatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.print_statment_syntax_node = [ &expressions ]( const State& state, const PrintStatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.f_syntax_node = [ &expressions ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.bin_expr_syntax_node = [ &expressions ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.un_expr_syntax_node = [ &expressions ]( const State& state, const UnExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.scope_statment_syntax_node = [ &expressions ]( const State& state, const ScopeSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        
                                                        handlers.function_statment_syntax_node = [ &expressions ]( const State& state, const FunctionStatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        
                                                        handlers.function_call_syntax_node = [ &expressions ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };

                                                        handlers.member_expression_syntax_node = [ &expressions ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.varible_assigment_statment_syntax_node = [ &expressions ]( const State& state, const VaribleAssigmentStatmentSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              expressions.emplace_back( node );
                                                              return { State::SCOPE_STATMENT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };

                                                        handlers.close_curly_bracket_syntax_node = [ &close_curly_bracket ]( const State& state, const CloseCurlyBracketSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::STATMENT )
                                                           {
                                                              close_curly_bracket = node;
                                                              return { State::FINISH, Impact::MOVE };
                                                           }
                                                           else if( state == State::SCOPE_STATMENT )
                                                           {
                                                              close_curly_bracket = node;
                                                              return { State::FINISH, Impact::MOVE };
                                                           }
                                                           else if( state == State::OPEN_CURLY_BRACKET )
                                                           {
                                                              close_curly_bracket = node;
                                                              return { State::FINISH, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
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
                                                            plan.to_remove.nodes.push_back( open_curly_bracket );
                                                            for( const auto& expression : expressions )
                                                               plan.to_remove.nodes.push_back( expression );
                                                            plan.to_remove.nodes.push_back( close_curly_bracket );

                                                            std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ open_curly_bracket->lexical_tokens().at(0), close_curly_bracket->lexical_tokens().at(0)  };
                                                            const auto& scope_node = std::make_shared< ScopeSyntaxNode >( expressions, scope_lexical_tokens );
                                                            plan.to_add.nodes.push_back( scope_node );
                                                            plan_or_progress = plan;
                                                        }
                                                        else {
                                                            plan_or_progress = Progress{ .readiness = iteration_result.readiness };
                                                        }
                                                        return plan_or_progress;
                                                        });
     } );

  // BOL SCOPE EOL
  mProductions.emplace_back(
     [ /* this */ ]( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress 
     {
        const size_t minimal_size = 3;
        size_t minimal_steps_number = 3;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number]( size_t n )->PlanOrProgress
                                                        {
                                                        BolSyntaxNodeSP bol;
                                                        ScopeSyntaxNodeSP scope;
                                                        EolSyntaxNodeSP eol;

                                                        const Stack& s = stack;
                                                        SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
                                                        using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
                                                        using HandlerReturn = Handlers::HandlerReturn;
                                                        using Impact = Handlers::Impact;

                                                         handlers.default_handler = []( const State& state, const ISyntaxNodeSP& ) -> HandlerReturn
                                                         { 
                                                            return { State::ERROR, Impact::ERROR };
                                                         };

                                                          handlers.bol_syntax_node = [ &bol ]( const State& state, const BolSyntaxNodeSP& node ) -> HandlerReturn
                                                          {
                                                             if( state == State::START )
                                                             {
                                                                bol = node;
                                                                return { State::BOL, Impact::MOVE };
                                                             }
                                                             return { state, Impact::NO_MOVE };
                                                          };
                                                          handlers.scope_statment_syntax_node = [ &scope ]( const State& state, const ScopeSyntaxNodeSP& node ) -> HandlerReturn
                                                          {
                                                             if( state == State::BOL )
                                                             {
                                                                scope = node;
                                                                return { State::SCOPE_STATMENT, Impact::MOVE };
                                                             }
                                                             return { state, Impact::NO_MOVE };
                                                          };
                                                          handlers.eol_syntax_node = [ &eol ]( const State& state, const EolSyntaxNodeSP& node ) -> HandlerReturn
                                                          {
                                                             if( state == State::SCOPE_STATMENT )
                                                             {
                                                                eol = node;
                                                                return { State::FINISH, Impact::MOVE };
                                                             }
                                                             return { state, Impact::NO_MOVE };
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
                                                            plan.to_remove.nodes.push_back( bol );
                                                            plan.to_remove.nodes.push_back( scope );
                                                            plan.to_remove.nodes.push_back( eol );

                                                            plan.to_add.nodes.push_back( scope );
                                                            plan_or_progress = plan;
                                                        }
                                                        else {
                                                            plan_or_progress = Progress{ .readiness = iteration_result.readiness };
                                                        }
                                                        return plan_or_progress;
                                                        });
     } );

}
