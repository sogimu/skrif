#include "member_expression_grammar.h"

#include "i_grammar.h"
#include "i_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "terminals/asterisk_syntax_node.h"
#include "terminals/dot_syntax_node.h"
#include "terminals/equal_syntax_node.h"
#include "terminals/int_syntax_node.h"
#include "terminals/less_syntax_node.h"
#include "terminals/more_syntax_node.h"
#include "terminals/open_square_bracket_syntax_node.h"
#include "terminals/close_square_bracket_syntax_node.h"
#include "terminals/name_syntax_node.h"
#include "terminals/plus_syntax_node.h"
#include "terminals/slash_syntax_node.h"
#include "terminals/string_syntax_node.h"
#include "utils.h"

#include <memory>
#include <vector>

class ISyntaxNodeVisitor;

MemberExpression::MemberExpression()
{
  enum class State
  {
     START,
     FINISH,
     ERROR,
     SOURCE,
     OPEN_SQUARE_BRACKET,
     CLOSE_SQUARE_BRACKET,
     STRING,
     KEY_OR_INDEX,
     MEMBER_EXPRESSION,
     DOT_STATE
  };

  // NAME OPEN_SQUARE_BRACKET INT|BIN_EXPR|FUNCTION_CALL|NAME|STRING CLOSE_SQUARE_BRACKET 
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 4;
        size_t minimal_steps_number = 4;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &lookahead, &minimal_steps_number]( size_t n ) -> PlanOrProgress
        {
            ISyntaxNodeSP source;
            OpenSquareBracketSyntaxNodeSP open_square_bracket;
            ISyntaxNodeSP key_or_index_node;
            CloseSquareBracketSyntaxNodeSP close_square_bracket;
            
            const Stack& s = stack;
            SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
            using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
            using HandlerReturn = Handlers::HandlerReturn;
            using Impact = Handlers::Impact;

            handlers.default_handler = []( const State& state, const ISyntaxNodeSP& ) -> HandlerReturn
            { 
               return { State::ERROR, Impact::ERROR };
            };
            handlers.name_syntax_node = [ &source, &key_or_index_node ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::START )
               {
                   std::vector< LexicalTokens::LexicalToken > lexical_tokens {};
                   source = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                   return { State::SOURCE, Impact::MOVE };
               }
               else if( state == State::OPEN_SQUARE_BRACKET )
               {
                   std::vector< LexicalTokens::LexicalToken > lexical_tokens {};
                   key_or_index_node = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                   return { State::KEY_OR_INDEX, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.member_expression_syntax_node = [ &source, &key_or_index_node ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::START )
               {
                   source = node;
                   return { State::SOURCE, Impact::MOVE };
               }
               else if( state == State::OPEN_SQUARE_BRACKET )
               {
                   key_or_index_node = node;
                   return { State::KEY_OR_INDEX, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.f_syntax_node = [ &key_or_index_node ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::OPEN_SQUARE_BRACKET )
               {
                   const auto& first_node = node->operator[](0);
                   if( check_type<IntSyntaxNode>( first_node ) || 
                       check_type<StringSyntaxNode>( first_node ) )
                   {
                       key_or_index_node = node;
                       return { State::KEY_OR_INDEX, Impact::MOVE };
                   }
               }
               return { state, Impact::ERROR };
            };
            handlers.bin_expr_syntax_node = [ &key_or_index_node ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::OPEN_SQUARE_BRACKET )
               {
                   key_or_index_node = node;
                   return { State::KEY_OR_INDEX, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.un_expr_syntax_node = [ &key_or_index_node ]( const State& state, const UnExprSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::OPEN_SQUARE_BRACKET )
               {
                   key_or_index_node = node;
                   return { State::KEY_OR_INDEX, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.function_call_syntax_node = [ &key_or_index_node ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::OPEN_SQUARE_BRACKET )
               {
                   key_or_index_node = node;
                   return { State::KEY_OR_INDEX, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.open_square_bracket_syntax_node = [ &open_square_bracket ]( const State& state, const OpenSquareBracketSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::SOURCE )
               {
                   open_square_bracket = node;
                   return { State::OPEN_SQUARE_BRACKET, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.close_square_bracket_syntax_node = [ &close_square_bracket, &lookahead ]( const State& state, const CloseSquareBracketSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::KEY_OR_INDEX )
               {
                 if( lookahead && ( check_type<SemicolonSyntaxNode>( lookahead ) || 
                                    check_type<CloseCircleBracketSyntaxNode>( lookahead ) ||
                                    check_type<CloseCurlyBracketSyntaxNode>( lookahead ) ||
                                    check_type<CloseSquareBracketSyntaxNode>( lookahead ) ||
                                    check_type<OpenSquareBracketSyntaxNode>( lookahead ) ||
                                    check_type<OpenCircleBracketSyntaxNode>( lookahead ) ||
                                    check_type<EqualSyntaxNode>( lookahead ) ||
                                    check_type<PlusSyntaxNode>( lookahead ) ||
                                    check_type<MinusSyntaxNode>( lookahead ) ||
                                    check_type<AsteriskSyntaxNode>( lookahead ) ||
                                    check_type<SlashSyntaxNode>( lookahead ) ||
                                    check_type<LessSyntaxNode>( lookahead ) ||
                                    check_type<MoreSyntaxNode>( lookahead ) ||
                                    check_type<NotSyntaxNode>( lookahead ) ||
                                    check_type<CommaSyntaxNode>( lookahead ) ) )
                 {
                     close_square_bracket = node;
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
                plan.to_remove.nodes.push_back( source );
                plan.to_remove.nodes.push_back( open_square_bracket );
                plan.to_remove.nodes.push_back( key_or_index_node );
                plan.to_remove.nodes.push_back( close_square_bracket );

                std::vector< LexicalTokens::LexicalToken > lexical_tokens { 
                                              source->lexical_tokens().at(0),
                                              open_square_bracket->lexical_tokens().at(0),
                                              close_square_bracket->lexical_tokens().at(0)
                                              };
                const auto& array = std::make_shared< MemberExpressionSyntaxNode >( source, key_or_index_node, lexical_tokens );
                plan.to_add.nodes.push_back( array );
                plan_or_progress = plan;
            }
            else {
                plan_or_progress = Progress{ .readiness = iteration_result.readiness };
            }
            return plan_or_progress;
        });
     } );
  
  // MEMBER_EXPRESSION SEMICOLON
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 2;
        size_t minimal_steps_number = 2;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number, &lookahead]( size_t n )->PlanOrProgress
                                                        
        {
          MemberExpressionSyntaxNodeSP member_expression;
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
          handlers.member_expression_syntax_node = [ &member_expression ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
          {
             if( state == State::START )
             {
                member_expression = node;
                return { State::MEMBER_EXPRESSION, Impact::MOVE };
             }
             return { state, Impact::ERROR };
          };
          handlers.semicolon_syntax_node = [ &semicolon ]( const State& state, const SemicolonSyntaxNodeSP& node ) -> HandlerReturn
          {
             if( state == State::MEMBER_EXPRESSION )
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
              plan.to_remove.nodes.push_back( member_expression );
              plan.to_remove.nodes.push_back( semicolon );

              plan.to_add.nodes.push_back( member_expression );
              plan_or_progress = plan;
          }
          else
          {
              plan_or_progress = Progress{ .readiness = iteration_result.readiness };
          }
          return plan_or_progress;
        });
     } );

  // SOURCE DOT NAME  (dot-notation property access: obj.key)
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 3;
        size_t minimal_steps_number = 3;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &lookahead, &minimal_steps_number]( size_t n ) -> PlanOrProgress
        {
            ISyntaxNodeSP source;
            DotSyntaxNodeSP dot;
            NameSyntaxNodeSP property_name;

            const Stack& s = stack;
            SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START };
            using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
            using HandlerReturn = Handlers::HandlerReturn;
            using Impact = Handlers::Impact;

            handlers.default_handler = []( const State& state, const ISyntaxNodeSP& ) -> HandlerReturn
            {
               return { State::ERROR, Impact::ERROR };
            };
            handlers.name_syntax_node = [ &source, &property_name, &lookahead ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::START )
               {
                   source = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                   return { State::SOURCE, Impact::MOVE };
               }
               else if( state == State::DOT_STATE )
               {
                   if( lookahead && ( check_type<SemicolonSyntaxNode>( lookahead ) ||
                                      check_type<CloseCircleBracketSyntaxNode>( lookahead ) ||
                                      check_type<CloseCurlyBracketSyntaxNode>( lookahead ) ||
                                      check_type<CloseSquareBracketSyntaxNode>( lookahead ) ||
                                      check_type<OpenSquareBracketSyntaxNode>( lookahead ) ||
                                      check_type<OpenCircleBracketSyntaxNode>( lookahead ) ||
                                      check_type<EqualSyntaxNode>( lookahead ) ||
                                      check_type<PlusSyntaxNode>( lookahead ) ||
                                      check_type<MinusSyntaxNode>( lookahead ) ||
                                      check_type<AsteriskSyntaxNode>( lookahead ) ||
                                      check_type<SlashSyntaxNode>( lookahead ) ||
                                      check_type<LessSyntaxNode>( lookahead ) ||
                                      check_type<MoreSyntaxNode>( lookahead ) ||
                                      check_type<NotSyntaxNode>( lookahead ) ||
                                      check_type<CommaSyntaxNode>( lookahead ) ||
                                      check_type<DotSyntaxNode>( lookahead ) ) )
                   {
                       property_name = node;
                       return { State::FINISH, Impact::MOVE };
                   }
               }
               return { state, Impact::ERROR };
            };
            handlers.member_expression_syntax_node = [ &source ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::START )
               {
                   source = node;
                   return { State::SOURCE, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.dot_syntax_node = [ &dot ]( const State& state, const DotSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::SOURCE )
               {
                   dot = node;
                   return { State::DOT_STATE, Impact::MOVE };
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
                plan.to_remove.nodes.push_back( source );
                plan.to_remove.nodes.push_back( dot );
                plan.to_remove.nodes.push_back( property_name );

                std::vector< LexicalTokens::LexicalToken > lexical_tokens {
                    source->lexical_tokens().at(0),
                    dot->lexical_tokens().at(0)
                };
                const auto& member_expr = std::make_shared< MemberExpressionSyntaxNode >( source, property_name, lexical_tokens );
                plan.to_add.nodes.push_back( member_expr );
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
