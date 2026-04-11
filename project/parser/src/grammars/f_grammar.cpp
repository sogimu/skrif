#include "f_grammar.h"

#include <vector>

#include "i_syntax_node.h"
#include "terminals/close_curly_bracket_syntax_node.h"
#include "terminals/comma_syntax_node.h"
#include "terminals/f_syntax_node.h"
#include "terminals/minus_syntax_node.h"
#include "terminals/double_syntax_node.h"
#include "terminals/semicolon_syntax_node.h"
#include "i_grammar.h"
#include "syntax_node_empty_visitor.h"
#include "utils.h"

F::F()
{
   enum class State
   {
      ERROR,
      START,
      FINISH,
      BOL,
      EOL,
      FIRST_MINUS,
      SECOND_MINUS,
      DOUBLE,
      INT,
      F,
      OPEN_CIRCLE_BRACKET,
      CLOSE_CIRCLE_BRACKET
   };

   // DOUBLE|INT|STRING [PLUS,MINUS,MULTIPLY,DIVISION,SEMICOLON,CLOSE_CIRCLE_BRACKET,COMMA,EQUAL,LESS,MORE,NOT]
   mProductions.emplace_back(
      []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
      {
         ISyntaxNodeSP number;
         FSyntaxNodeSP f_node;

         ProgressCounter progress_counter{1};

         State state = State::START;
         SyntaxNodeEmptyVisitor::Handlers handlers;
         handlers.default_handler = [ &state ]( const ISyntaxNodeSP& ) { state = State::ERROR; };
         handlers.double_syntax_node =
             [ &f_node, &number, &state, &lookahead, &progress_counter ]( const DoubleSyntaxNodeSP& node )
          {
             if( state == State::START )
             {
                state = State::DOUBLE;

                if( lookahead && ( 
                     check_type<SemicolonSyntaxNode>( lookahead ) || 
                     check_type<MinusSyntaxNode>( lookahead ) || 
                     check_type<PlusSyntaxNode>( lookahead ) || 
                     check_type<AsteriskSyntaxNode>( lookahead ) || 
                     check_type<CloseCircleBracketSyntaxNode>( lookahead ) || 
                     check_type<CloseCurlyBracketSyntaxNode>( lookahead ) || 
                     check_type<CloseSquareBracketSyntaxNode>( lookahead ) || 
                     check_type<SlashSyntaxNode>( lookahead ) || 
                     check_type<CommaSyntaxNode>( lookahead ) ||
                     check_type<EqualSyntaxNode>( lookahead ) ||
                     check_type<LessSyntaxNode>( lookahead ) ||
                     check_type<MoreSyntaxNode>( lookahead ) ||
                     check_type<NotSyntaxNode>( lookahead ) ) )
                {
                  number = node;
                  f_node = std::make_shared< FSyntaxNode >( node );
                  progress_counter.Step();
                  state = State::FINISH;
                }
             }
          };
          handlers.int_syntax_node =
             [ &f_node, &number, &state, &lookahead, &progress_counter ]( const IntSyntaxNodeSP& node )
          {
             if( state == State::START )
             {
                state = State::INT;

                if( lookahead && ( 
                     check_type<SemicolonSyntaxNode>( lookahead ) || 
                     check_type<MinusSyntaxNode>( lookahead ) || 
                     check_type<PlusSyntaxNode>( lookahead ) || 
                     check_type<AsteriskSyntaxNode>( lookahead ) || 
                     check_type<CloseCircleBracketSyntaxNode>( lookahead ) || 
                     check_type<CloseCurlyBracketSyntaxNode>( lookahead ) || 
                     check_type<CloseSquareBracketSyntaxNode>( lookahead ) || 
                     check_type<SlashSyntaxNode>( lookahead ) || 
                     check_type<CommaSyntaxNode>( lookahead ) ||
                     check_type<EqualSyntaxNode>( lookahead ) ||
                     check_type<LessSyntaxNode>( lookahead ) ||
                     check_type<MoreSyntaxNode>( lookahead ) ||
                     check_type<NotSyntaxNode>( lookahead ) ) )
                {
                  number = node;
                  f_node = std::make_shared< FSyntaxNode >( node );
                  progress_counter.Step();
                  state = State::FINISH;
                }
             }
          };
          handlers.string_syntax_node =
             [ &f_node, &number, &state, &lookahead, &progress_counter ]( const StringSyntaxNodeSP& node )
          {
             auto& s = state;
             (void) s;
             auto& l = lookahead;
             (void) l;
             if( state == State::START )
             {
                state = State::INT;

                if( lookahead && ( 
                     check_type<SemicolonSyntaxNode>( lookahead ) || 
                     check_type<MinusSyntaxNode>( lookahead ) || 
                     check_type<PlusSyntaxNode>( lookahead ) || 
                     check_type<AsteriskSyntaxNode>( lookahead ) || 
                     check_type<CloseCircleBracketSyntaxNode>( lookahead ) || 
                     check_type<CloseCurlyBracketSyntaxNode>( lookahead ) || 
                     check_type<CloseSquareBracketSyntaxNode>( lookahead ) || 
                     check_type<SlashSyntaxNode>( lookahead ) || 
                     check_type<CommaSyntaxNode>( lookahead ) ||
                     check_type<EqualSyntaxNode>( lookahead ) ||
                     check_type<LessSyntaxNode>( lookahead ) ||
                     check_type<MoreSyntaxNode>( lookahead ) ) )
                {
                  number = node;
                  f_node = std::make_shared< FSyntaxNode >( node );
                  progress_counter.Step();
                  state = State::FINISH;
                }
             }
          };
         iterate_over_last_n_nodes( stack, 1, handlers );
         if( state != State::FINISH )
            return Progress{ .readiness = progress_counter.Status() };

         Plan plan;
         plan.to_remove.nodes.push_back( number );

         plan.to_add.nodes.push_back( f_node );
         return plan;
      } );
   
   // F SEMICOLON
   mProductions.emplace_back(
      []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
      {
         const size_t minimal_size = 2;
         size_t minimal_steps_number = 2;
         return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number, &lookahead]( size_t n )->PlanOrProgress
                                                         
         {
           FSyntaxNodeSP f;
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
           handlers.f_syntax_node = [ &f ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
           {
              if( state == State::START )
              {
                 f = node;
                 return { State::F, Impact::MOVE };
              }
              return { state, Impact::ERROR };
           };
           handlers.semicolon_syntax_node = [ &semicolon ]( const State& state, const SemicolonSyntaxNodeSP& node ) -> HandlerReturn
           {
              if( state == State::F )
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
               plan.to_remove.nodes.push_back( f );
               plan.to_remove.nodes.push_back( semicolon );

               plan.to_add.nodes.push_back( f );
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
