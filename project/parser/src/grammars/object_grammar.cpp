#include "object_grammar.h"

#include "i_grammar.h"
#include "i_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "nonterminals/object_syntax_node.h"
#include "terminals/f_syntax_node.h"
#include "terminals/open_square_bracket_syntax_node.h"
#include "terminals/close_square_bracket_syntax_node.h"
#include "terminals/comma_syntax_node.h"
#include "terminals/colon_syntax_node.h"
#include "terminals/string_syntax_node.h"
#include "utils.h"

#include <memory>
#include <vector>

class ISyntaxNodeVisitor;

Object::Object()
{
  enum class State
  {
     START,
     FINISH,
     ERROR,
     OPEN_CURLY_BRACKET,
     CLOSE_CURLY_BRACKET,
     VALUE,
     COMMA,
     COLON,
     STRING,
     PAIR,
     OBJECT
  };

  // OPEN_CURLY_BRACKET OBJECT_PAIR+ CLOSE_CURLY_BRACKET 
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        bool is_open_curly_bracket_found = false;
        bool is_close_curly_bracket_found = false;
        size_t distance_between_open_close_curly_bracket = 0;
        
        auto it = stack.rbegin();
        SyntaxNodeEmptyVisitor::Handlers open_curly_bracket_handler;
        for( ; it != stack.rend(); ++it )
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
        
        if( !is_open_curly_bracket_found )
            return Progress{ .readiness = 0.0 };
        
        SyntaxNodeEmptyVisitor::Handlers close_curly_bracket_handler;
        for( ; it != stack.rend(); ++it )
        {
           close_curly_bracket_handler.close_curly_bracket_syntax_node = [ &is_close_curly_bracket_found ]( const CloseCurlyBracketSyntaxNodeSP& /* node */ )
           { is_close_curly_bracket_found = true; };
           const auto& close_curly_bracket_visitor = std::make_shared< SyntaxNodeEmptyVisitor >( close_curly_bracket_handler );
           ( *it )->accept( close_curly_bracket_visitor );
           if( is_close_curly_bracket_found )
           {
              break;
           }
        }

        const size_t minimal_size = distance_between_open_close_curly_bracket + 1;
        size_t minimal_steps_number = 0;
        if( is_open_curly_bracket_found && !is_close_curly_bracket_found )
            minimal_steps_number = minimal_size + 1;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &lookahead, &minimal_steps_number]( size_t n ) -> PlanOrProgress
        {
            OpenCurlyBracketSyntaxNodeSP open_curly_bracket;
            std::vector< ObjectPairSyntaxNodeSP > pairs;
            std::vector< CommaSyntaxNodeSP > commas;
            CloseCurlyBracketSyntaxNodeSP close_curly_bracket;

            const Stack& s = stack;
            SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
            using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
            using HandlerReturn = Handlers::HandlerReturn;
            using Impact = Handlers::Impact;

            handlers.default_handler = []( const State& state, const ISyntaxNodeSP& node ) -> HandlerReturn
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
               return { state, Impact::ERROR };
            };
            handlers.object_pair_syntax_node = [ &pairs ]( const State& state, const ObjectPairSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::OPEN_CURLY_BRACKET || state == State::COMMA ) 
               {
                   pairs.emplace_back( node );
                   return { State::PAIR, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.comma_syntax_node = [ &commas ]( const State& state, const CommaSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::PAIR )
               {
                   commas.emplace_back( node );
                   return { State::COMMA, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.close_curly_bracket_syntax_node = [ &close_curly_bracket, &lookahead ]( const State& state, const CloseCurlyBracketSyntaxNodeSP& node ) -> HandlerReturn
            {
                 auto lwef = lookahead;
                   (void) lwef;
               lwef.get();
              
               if( state == State::PAIR || state == State::OPEN_CURLY_BRACKET )
               {
                 if( lookahead && ( check_type<SemicolonSyntaxNode>( lookahead ) || 
                                    check_type<CloseSquareBracketSyntaxNode>( lookahead ) ||
                                    check_type<CloseCurlyBracketSyntaxNode>( lookahead ) ||
                                    check_type<CloseCircleBracketSyntaxNode>( lookahead ) ||
                                    check_type<CommaSyntaxNode>( lookahead ) ) )
                 {
                     close_curly_bracket = node;
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
                plan.to_remove.nodes.push_back( open_curly_bracket );
                for( const auto& comma : commas )
                   plan.to_remove.nodes.push_back( comma );
                for( const auto& pair : pairs )
                   plan.to_remove.nodes.push_back( pair );
                plan.to_remove.nodes.push_back( close_curly_bracket );

                std::vector< LexicalTokens::LexicalToken > lexical_tokens { 
                                              open_curly_bracket->lexical_tokens().at(0),
                                              close_curly_bracket->lexical_tokens().at(0)
                                              };
                const auto& object = std::make_shared< ObjectSyntaxNode >( pairs, lexical_tokens );
                plan.to_add.nodes.push_back( object );
                plan_or_progress = plan;
            }
            else {
                plan_or_progress = Progress{ .readiness = iteration_result.readiness };
            }
            return plan_or_progress;
        });
     } );
  
  // STRING COLON F|ARRAY|OBJECT|FUNCTION_CALL|BIN_EXPR|NAME|MEMBER_EXPRESSION 
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 3;
        size_t minimal_steps_number = 3;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &lookahead, &minimal_steps_number]( size_t n ) -> PlanOrProgress
        {
            StringSyntaxNodeSP key;
            ColonSyntaxNodeSP colon;
            ISyntaxNodeSP value;

            const Stack& s = stack;
            SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_steps_number, State::START};
            using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
            using HandlerReturn = Handlers::HandlerReturn;
            using Impact = Handlers::Impact;

            handlers.default_handler = []( const State& state, const ISyntaxNodeSP& node ) -> HandlerReturn
            { 
                                               (void) node;
               return { State::ERROR, Impact::ERROR };
            };
            handlers.string_syntax_node = [ &key, &lookahead ]( const State& state, const StringSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::START ) 
               {
                   key = node;
                   return { State::STRING, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.colon_syntax_node = [ &colon ]( const State& state, const ColonSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::STRING )
               {
                   colon = node;
                   return { State::COLON, Impact::MOVE };
               }
               return { state, Impact::ERROR };
            };
            handlers.f_syntax_node = [ &value, &lookahead ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     value = node;
                     return { State::FINISH, Impact::MOVE };
                  }
               }
               return { state, Impact::ERROR };
            };
            handlers.name_syntax_node = [ &value, &lookahead ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     std::vector< LexicalTokens::LexicalToken > lexical_tokens {};
                     value = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                     return { State::FINISH, Impact::MOVE };
                  }
               }
               return { state, Impact::ERROR };
            };
            handlers.bin_expr_syntax_node = [ &value, &lookahead ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     value = node;
                     return { State::FINISH, Impact::MOVE };
                  }
               }
               return { state, Impact::ERROR };
            };
            handlers.function_call_syntax_node = [ &value, &lookahead ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     value = node;
                     return { State::FINISH, Impact::MOVE };
                  }
               }
               return { state, Impact::ERROR };
            };
            handlers.member_expression_syntax_node = [ &value, &lookahead ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     value = node;
                     return { State::FINISH, Impact::MOVE };
                  }
               }
               return { state, Impact::ERROR };
            };
            handlers.array_syntax_node = [ &value, &lookahead ]( const State& state, const ArraySyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     value = node;
                     return { State::FINISH, Impact::MOVE };
                  }
               }
               return { state, Impact::ERROR };
            };
            handlers.object_syntax_node = [ &value, &lookahead ]( const State& state, const ObjectSyntaxNodeSP& node ) -> HandlerReturn
            {
               if( state == State::COLON )
               {
                  auto& s = lookahead;
                  (void) lookahead;
                  if( lookahead && check_type< CommaSyntaxNode >( lookahead ) ||
                                   check_type< CloseCurlyBracketSyntaxNode >( lookahead ) ) 
                  {
                     value = node;
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
                plan.to_remove.nodes.push_back( key );
                plan.to_remove.nodes.push_back( colon );
                plan.to_remove.nodes.push_back( value );

                std::vector< LexicalTokens::LexicalToken > lexical_tokens { 
                                              colon->lexical_tokens().at(0),
                                              };
                const auto& object_pair = std::make_shared< ObjectPairSyntaxNode >( key, value, lexical_tokens );
                plan.to_add.nodes.push_back( object_pair );
                plan_or_progress = plan;
            }
            else {
                plan_or_progress = Progress{ .readiness = iteration_result.readiness };
            }
            return plan_or_progress;
        });
     } );
  
  // OBJECT SEMICOLON
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 2;
        size_t minimal_steps_number = 2;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number, &lookahead]( size_t n )->PlanOrProgress
                                                        
        {
          ObjectSyntaxNodeSP object;
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
          handlers.object_syntax_node = [ &object ]( const State& state, const ObjectSyntaxNodeSP& node ) -> HandlerReturn
          {
             if( state == State::START )
             {
                object = node;
                return { State::OBJECT, Impact::MOVE };
             }
             return { state, Impact::ERROR };
          };
          handlers.semicolon_syntax_node = [ &semicolon ]( const State& state, const SemicolonSyntaxNodeSP& node ) -> HandlerReturn
          {
             if( state == State::OBJECT )
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
              plan.to_remove.nodes.push_back( object );
              plan.to_remove.nodes.push_back( semicolon );

              plan.to_add.nodes.push_back( object );
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
