#include "bin_expr_grammar.h"

#include "lexical_tokens.h"
#include "nonterminals/bin_expr_syntax_node.h"
#include "nonterminals/function_call_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "terminals/asterisk_syntax_node.h"
#include "terminals/close_curly_bracket_syntax_node.h"
#include "terminals/comma_syntax_node.h"
#include "terminals/less_syntax_node.h"
#include "terminals/minus_syntax_node.h"
#include "terminals/more_syntax_node.h"
#include "terminals/name_syntax_node.h"
#include "i_grammar.h"
#include "i_syntax_node.h"
#include "nonterminals/print_statment_syntax_node.h"
#include "syntax_node_empty_visitor.h"
#include "terminals/plus_syntax_node.h"
#include "terminals/slash_syntax_node.h"
#include "utils.h"

#include <memory>
#include <vector>

BinExpr::BinExpr()
{
  enum class State
  {
     START,
     FINISH,
     ERROR,
     ADDITION,
     SUBTRACTION,
     MULTIPLY,
     DIVISION,
     PLUS,
     MINUS,
     F,
     BIN_EXPR,
     UN_EXPR,
     EQUAL_EXPRESSION0,
     EQUAL_EXPRESSION1,
     NOT,
     NOT_EQUAL,
     MORE,
     MORE_EQUAL,
     LESS,
     LESS_EQUAL,
     OPEN_CIRCLE_BRACKET,
     NAME,
     CLOSE_CIRCLE_BRACKET,
     BOL,
     EOL,
     ANY_NODE
  };
  
  // F|BIN_EXPR|UN_EXPR|NAME|FUNCTION_CALL|MEMBER_EXPRESSION +|-|*|/ F|BIN_EXPR|UN_EXPR|NAME|FUNCTION_CALL|MEMBER_EXPRESSION [SEMICOLON|CLOSE_CIRCLE_BRACKET,BIN_EXPR,COMMA,EQUAL] 
  mProductions.emplace_back(
     [ /* this */ ]( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 3;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_size, &lookahead]( size_t n )->PlanOrProgress
                                                        {
                                                        ISyntaxNodeSP a;
                                                        ISyntaxNodeSP operation;
                                                        ISyntaxNodeSP b;
                                                        BinExprSyntaxNode::Type operation_type; 

                                                        const Stack& s = stack;
                                                        SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_size, State::START};
                                                        using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
                                                        using HandlerReturn = Handlers::HandlerReturn;
                                                        using Impact = Handlers::Impact;

                                                        handlers.default_handler = []( const State& state, const ISyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                            return { State::ERROR, Impact::ERROR };
                                                        };
                                                        handlers.f_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::PLUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Addition;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ))
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MINUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Substruction;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MULTIPLY )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Multiply;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::DIVISION )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Division;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.bin_expr_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::PLUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Addition;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead )  || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ))
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MINUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Substruction;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead )  || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ))
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MULTIPLY )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Multiply;
                                                              if( lookahead && ( check_type< SemicolonSyntaxNode >( lookahead )  || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::DIVISION )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Division;
                                                              if( lookahead && ( check_type< SemicolonSyntaxNode >( lookahead )  || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< BinExprSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.un_expr_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const UnExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::PLUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Addition;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MINUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Substruction;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MULTIPLY )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Multiply;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::DIVISION )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Division;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.name_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                               a = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                               return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::PLUS )
                                                           {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                              operation_type = BinExprSyntaxNode::Type::Addition;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MINUS )
                                                           {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                              operation_type = BinExprSyntaxNode::Type::Substruction;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MULTIPLY )
                                                           {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                              operation_type = BinExprSyntaxNode::Type::Multiply;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::DIVISION )
                                                           {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                              operation_type = BinExprSyntaxNode::Type::Division;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.function_call_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::PLUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Addition;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MINUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Substruction;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MULTIPLY )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Multiply;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::DIVISION )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Division;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.member_expression_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::PLUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Addition;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MINUS )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Substruction;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MULTIPLY )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Multiply;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::DIVISION )
                                                           {
                                                              b = node;
                                                              operation_type = BinExprSyntaxNode::Type::Division;
                                                              if( lookahead && check_type< SemicolonSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCircleBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseCurlyBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< CloseSquareBracketSyntaxNode >( lookahead ) || 
                                                                               check_type< PlusSyntaxNode >( lookahead ) || 
                                                                               check_type< MinusSyntaxNode >( lookahead ) || 
                                                                               check_type< CommaSyntaxNode >( lookahead ) || 
                                                                               check_type< EqualSyntaxNode >( lookahead ) || 
                                                                               check_type< LessSyntaxNode >( lookahead ) || 
                                                                               check_type< MoreSyntaxNode >( lookahead ) )
                                                              {
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.plus_syntax_node = [ &operation, &operation_type ]( const State& state, const PlusSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              operation = node;
                                                              return { State::PLUS, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.minus_syntax_node = [ &operation, &operation_type ]( const State& state, const MinusSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              operation = node;
                                                              return { State::MINUS, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.asterisk_syntax_node = [ &operation, &operation_type ]( const State& state, const AsteriskSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              operation = node;
                                                              return { State::MULTIPLY, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.slash_syntax_node = [ &operation, &operation_type ]( const State& state, const SlashSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              operation = node;
                                                              return { State::DIVISION, Impact::MOVE };
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
                                                            plan.to_remove.nodes.push_back( a );
                                                            plan.to_remove.nodes.push_back( operation );
                                                            plan.to_remove.nodes.push_back( b );
                                                            std::vector<LexicalTokens::LexicalToken> lexical_tokens{ operation->lexical_tokens().at(0) };
                                                            const auto& expression_node = std::make_shared< BinExprSyntaxNode >(operation_type, a, b, lexical_tokens );

                                                            plan.to_add.nodes.push_back( expression_node );
                                                            plan_or_progress = plan;
                                                        }
                                                        else {
                                                            plan_or_progress = Progress{ .readiness = iteration_result.readiness };
                                                        }
                                                        return plan_or_progress;
                                                        });
     } );

  // F|BIN_EXPR|UN_EXPR|NAME|FUNCTION_CALL|MEMBER_EXPRESSION (EQUAL EQUAL) | (NOT EQUAL) | (LESS EQUAL) | (MORE EQUAL) F|BIN_EXPR|UN_EXPR|NAME|FUNCTION_CALL|MEMBER_EXPRESSION [!PLUS&&!MINUS&&!ASTERIX&&!SLASH] 
  mProductions.emplace_back(
     [ /* this */ ]( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        auto & s = stack;
        (void) s;
        const size_t minimal_size = 4;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_size, &lookahead]( size_t n )->PlanOrProgress
                                                        {
                                                        ISyntaxNodeSP a;
                                                        EqualSyntaxNodeSP equal0;
                                                        EqualSyntaxNodeSP equal1;
                                                        NotSyntaxNodeSP not_node;
                                                        MoreSyntaxNodeSP more;
                                                        LessSyntaxNodeSP less;
                                                        ISyntaxNodeSP b;
                                                        BinExprSyntaxNode::Type operation_type; 

                                                        const Stack& s = stack;
                                                        SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_size, State::START};
                                                        using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
                                                        using HandlerReturn = Handlers::HandlerReturn;
                                                        using Impact = Handlers::Impact;

                                                        handlers.default_handler = []( const State& state, const ISyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                            return { State::ERROR, Impact::ERROR };
                                                        };
                                                        handlers.f_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION1 )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::Equal;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MORE_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::MoreEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::LessEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::NOT_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::NotEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.bin_expr_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION1 )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::Equal;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MORE_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::MoreEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::LessEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::NOT_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::NotEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.un_expr_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const UnExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION1 )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::Equal;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MORE_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::MoreEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::LessEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::NOT_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::NotEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.name_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                               a = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION1 )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) && 
                                                                               !check_type< OpenSquareBracketSyntaxNode >( lookahead ) )
                                                              {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                                operation_type = BinExprSyntaxNode::Type::Equal;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MORE_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) && 
                                                                               !check_type< OpenSquareBracketSyntaxNode >( lookahead ) )
                                                              {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                                operation_type = BinExprSyntaxNode::Type::MoreEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) && 
                                                                               !check_type< OpenSquareBracketSyntaxNode >( lookahead ) )
                                                              {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                                operation_type = BinExprSyntaxNode::Type::LessEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::NOT_EQUAL )
                                                           {
                                                              auto& ss = lookahead;
                                                              (void) ss;
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) && 
                                                                               !check_type< OpenSquareBracketSyntaxNode >( lookahead ) )
                                                              {
                                                               b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                                                                operation_type = BinExprSyntaxNode::Type::NotEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.function_call_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION1 )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::Equal;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MORE_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::MoreEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::LessEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::NOT_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::NotEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.member_expression_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION1 )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::Equal;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::MORE_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::MoreEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::LessEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::NOT_EQUAL )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::NotEqual;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.equal_syntax_node = [ &equal0, &equal1 ]( const State& state, const EqualSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              equal0 = node;
                                                              return { State::EQUAL_EXPRESSION0, Impact::MOVE };
                                                           }
                                                           else if( state == State::EQUAL_EXPRESSION0 )
                                                           {
                                                              equal1 = node;
                                                              return { State::EQUAL_EXPRESSION1, Impact::MOVE };
                                                           }
                                                           else if( state == State::NOT )
                                                           {
                                                              equal1 = node;
                                                              return { State::NOT_EQUAL, Impact::MOVE };
                                                           }
                                                           else if( state == State::LESS )
                                                           {
                                                              equal1 = node;
                                                              return { State::LESS_EQUAL, Impact::MOVE };
                                                           }
                                                           else if( state == State::MORE )
                                                           {
                                                              equal1 = node;
                                                              return { State::MORE_EQUAL, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.not_syntax_node = [ &not_node ]( const State& state, const NotSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              not_node = node;
                                                              return { State::NOT, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.more_syntax_node = [ &more, &equal1 ]( const State& state, const MoreSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              more = node;
                                                              return { State::MORE, Impact::MOVE };
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.less_syntax_node = [ &less, &equal1 ]( const State& state, const LessSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::F )
                                                           {
                                                              less = node;
                                                              return { State::LESS, Impact::MOVE };
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
                                                            ISyntaxNodeSP first_operation_node;
                                                            ISyntaxNodeSP second_operation_node;
                                                            std::vector< ISyntaxNodeSP > nodes;
                                                            nodes.push_back( a );
                                                            if( operation_type == BinExprSyntaxNode::Type::Equal )
                                                            {
                                                              nodes.push_back( equal0 );
                                                              nodes.push_back( equal1 );
                                                              first_operation_node = equal0;
                                                              second_operation_node = equal1;
                                                            }
                                                            else if( operation_type == BinExprSyntaxNode::Type::NotEqual )
                                                            {
                                                              nodes.push_back( not_node );
                                                              nodes.push_back( equal1 );
                                                              first_operation_node = not_node;
                                                              second_operation_node = equal1;
                                                            }
                                                            else if( operation_type == BinExprSyntaxNode::Type::LessEqual )
                                                            {
                                                              nodes.push_back( less );
                                                              nodes.push_back( equal1 );
                                                              first_operation_node = less;
                                                              second_operation_node = equal1;
                                                            }
                                                            else if( operation_type == BinExprSyntaxNode::Type::MoreEqual )
                                                            {
                                                              nodes.push_back( more );
                                                              nodes.push_back( equal1 );
                                                              first_operation_node = more;
                                                              second_operation_node = equal1;
                                                            }
                                                            nodes.push_back( b );

                                                            Plan plan;
                                                            plan.to_remove.nodes = nodes;

                                                            std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ first_operation_node->lexical_tokens().at(0), second_operation_node->lexical_tokens().at(0) };
                                                            const auto& expression_node = std::make_shared< BinExprSyntaxNode >(operation_type, a, b, bin_expr_lexical_tokens );
                                                            plan.to_add.nodes.push_back( expression_node );
                                                            plan_or_progress = plan;
                                                        }
                                                        else
                                                        {
                                                            plan_or_progress = Progress{ .readiness = iteration_result.readiness };
                                                        }
                                                        return plan_or_progress;
                                                        });
     } );
  
  // F|BIN_EXPR|UN_EXPR|NAME|FUNCTION_CALL|MEMBER_EXPRESSION LESS|MORE F|BIN_EXPR|UN_EXPR|NAME|FUNCTION_CALL|MEMBER_EXPRESSION [!PLUS&&!MINUS&&!ASTERIX&&!SLASH] 
  mProductions.emplace_back(
     [ /* this */ ]( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {

        const size_t minimal_size = 3;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_size, &lookahead]( size_t n )->PlanOrProgress
                                                        {
                                                        ISyntaxNodeSP a;
                                                        MoreSyntaxNodeSP more;
                                                        LessSyntaxNodeSP less;
                                                        ISyntaxNodeSP b;
                                                        BinExprSyntaxNode::Type operation_type; 

                                                        const Stack& s = stack;
                                                        SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_size, State::START};
                                                        using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
                                                        using HandlerReturn = Handlers::HandlerReturn;
                                                        using Impact = Handlers::Impact;

                                                        handlers.default_handler = []( const State& state, const ISyntaxNodeSP& node ) -> HandlerReturn
                                                        { 
                                                           return { State::ERROR, Impact::NO_MOVE };
                                                        };
                                                        handlers.f_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const FSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
                                                              a = node;
                                                              return { State::F, Impact::MOVE };
                                                           }
                                                           else if( state == State::MORE )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::More;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           else if( state == State::LESS )
                                                           {
                                                              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                                                               !check_type< MinusSyntaxNode >( lookahead ) && 
                                                                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                                                               !check_type< SlashSyntaxNode >( lookahead ) )
                                                              {
                                                                b = node;
                                                                operation_type = BinExprSyntaxNode::Type::Less;
                                                                return { State::FINISH, Impact::MOVE };
                                                              }
                                                           }
                                                           return { state, Impact::NO_MOVE };
                                                        };
                                                        handlers.bin_expr_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
                                                        {
                                                           if( state == State::START )
                                                           {
              a = node;
              return { State::F, Impact::MOVE };
           }
           else if( state == State::MORE )
           {
              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                               !check_type< MinusSyntaxNode >( lookahead ) && 
                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                               !check_type< SlashSyntaxNode >( lookahead ) )
              {
                b = node;
                operation_type = BinExprSyntaxNode::Type::More;
                return { State::FINISH, Impact::MOVE };
              }
           }
           else if( state == State::LESS )
           {
              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                               !check_type< MinusSyntaxNode >( lookahead ) && 
                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                               !check_type< SlashSyntaxNode >( lookahead ) )
              {
                b = node;
                operation_type = BinExprSyntaxNode::Type::Less;
                return { State::FINISH, Impact::MOVE };
              }
           }
           return { state, Impact::NO_MOVE };
        };
        handlers.un_expr_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const UnExprSyntaxNodeSP& node ) -> HandlerReturn
        {
           if( state == State::START )
           {
              a = node;
              return { State::F, Impact::MOVE };
           }
           else if( state == State::MORE )
           {
              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                               !check_type< MinusSyntaxNode >( lookahead ) && 
                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                               !check_type< SlashSyntaxNode >( lookahead ) )
              {
                b = node;
                operation_type = BinExprSyntaxNode::Type::More;
                return { State::FINISH, Impact::MOVE };
              }
           }
           else if( state == State::LESS )
           {
              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                               !check_type< MinusSyntaxNode >( lookahead ) && 
                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                               !check_type< SlashSyntaxNode >( lookahead ) )
              {
                b = node;
                operation_type = BinExprSyntaxNode::Type::Less;
                return { State::FINISH, Impact::MOVE };
              }
           }
           return { state, Impact::NO_MOVE };
        };
        handlers.name_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const NameSyntaxNodeSP& node ) -> HandlerReturn
        {
           if( state == State::START )
           {
               a = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
              return { State::F, Impact::MOVE };
           }
           else if( state == State::MORE )
           {
              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                               !check_type< MinusSyntaxNode >( lookahead ) && 
                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                               !check_type< SlashSyntaxNode >( lookahead ) )
              {
                b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                operation_type = BinExprSyntaxNode::Type::More;
                return { State::FINISH, Impact::MOVE };
              }
           }
           else if( state == State::LESS )
           {
              if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                               !check_type< MinusSyntaxNode >( lookahead ) && 
                               !check_type< AsteriskSyntaxNode >( lookahead ) && 
                               !check_type< SlashSyntaxNode >( lookahead ) )
              {
                b = std::make_shared<VaribleSyntaxNode>( node, node->lexical_tokens() );
                operation_type = BinExprSyntaxNode::Type::Less;
                return { State::FINISH, Impact::MOVE };
              }
           }
           return { state, Impact::NO_MOVE };
        };
        handlers.function_call_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const FunctionCallSyntaxNodeSP& node ) -> HandlerReturn
        {
            if( state == State::START )
            {
               a = node;
              return { State::F, Impact::MOVE };
            }
            else if( state == State::MORE )
            {
               if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                !check_type< MinusSyntaxNode >( lookahead ) && 
                                !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                !check_type< SlashSyntaxNode >( lookahead ) )
               {
                 b = node;
                 operation_type = BinExprSyntaxNode::Type::More;
                 return { State::FINISH, Impact::MOVE };
               }
            }
            else if( state == State::LESS )
            {
               if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                !check_type< MinusSyntaxNode >( lookahead ) && 
                                !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                !check_type< SlashSyntaxNode >( lookahead ) )
               {
                 b = node;
                 operation_type = BinExprSyntaxNode::Type::Less;
                 return { State::FINISH, Impact::MOVE };
               }
            }
           return { state, Impact::NO_MOVE };
        };
        handlers.member_expression_syntax_node = [ &operation_type, &a, &b, &lookahead ]( const State& state, const MemberExpressionSyntaxNodeSP& node ) -> HandlerReturn
        {
            if( state == State::START )
            {
               a = node;
              return { State::F, Impact::MOVE };
            }
            else if( state == State::MORE )
            {
               if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                !check_type< MinusSyntaxNode >( lookahead ) && 
                                !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                !check_type< SlashSyntaxNode >( lookahead ) )
               {
                 b = node;
                 operation_type = BinExprSyntaxNode::Type::More;
                 return { State::FINISH, Impact::MOVE };
               }
            }
            else if( state == State::LESS )
            {
               if( lookahead && !check_type< PlusSyntaxNode >( lookahead ) && 
                                !check_type< MinusSyntaxNode >( lookahead ) && 
                                !check_type< AsteriskSyntaxNode >( lookahead ) && 
                                !check_type< SlashSyntaxNode >( lookahead ) )
               {
                 b = node;
                 operation_type = BinExprSyntaxNode::Type::Less;
                 return { State::FINISH, Impact::MOVE };
               }
            }
           return { state, Impact::NO_MOVE };
        };
        handlers.more_syntax_node = [ &more ]( const State& state, const MoreSyntaxNodeSP& node ) -> HandlerReturn
        {
           if( state == State::F )
           {
              more = node;
              return { State::MORE, Impact::MOVE };
           }
           return { state, Impact::NO_MOVE };
        };
        handlers.less_syntax_node = [ &less ]( const State& state, const LessSyntaxNodeSP& node ) -> HandlerReturn
        {
           if( state == State::F )
           {
              less = node;
              return { State::LESS, Impact::MOVE };
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
            ISyntaxNodeSP operation_syntax_node;
            Plan plan;
            plan.to_remove.nodes.push_back( a );
            if( operation_type == BinExprSyntaxNode::Type::Less )
            {
                operation_syntax_node = less;
                plan.to_remove.nodes.push_back( less );
            }
            else if( operation_type == BinExprSyntaxNode::Type::More )
            {
                operation_syntax_node = more;
                plan.to_remove.nodes.push_back( more );
            }
            plan.to_remove.nodes.push_back( b );

            std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ operation_syntax_node->lexical_tokens().at(0) };
            const auto& expression_node = std::make_shared< BinExprSyntaxNode >(operation_type, a, b, bin_expr_lexical_tokens );

            plan.to_add.nodes.push_back( expression_node );
            plan_or_progress = plan;
        }
        else
        {
            plan_or_progress = Progress{ .readiness = iteration_result.readiness };
        }
        return plan_or_progress;
        });
     } );

    mProductions.emplace_back(
       [ /* this */ ]( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
       {
          const size_t minimal_size = 3;
          return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_size]( size_t n )->PlanOrProgress
                                                          {
                                                          BolSyntaxNodeSP bol;
                                                          ISyntaxNodeSP statment;
                                                          EolSyntaxNodeSP eol;

                                                          SyntaxNodeProgressVisitor< State >::Handlers handlers{ minimal_size, State::START};
                                                          using Handlers = SyntaxNodeProgressVisitor<State>::Handlers;
                                                          using HandlerReturn = Handlers::HandlerReturn;
                                                          using Impact = Handlers::Impact;
                                                          handlers.bol_syntax_node = [ &bol, &statment ]( const State& state, const BolSyntaxNodeSP& node ) -> HandlerReturn
                                                          {
                                                             if( state == State::START )
                                                             {
                                                                bol = node;
                                                                return {State::BOL, Impact::MOVE};
                                                             }
                                                             return {state, Impact::ERROR};
                                                          };
                                                          handlers.default_handler = [&statment]( const State& state, const ISyntaxNodeSP& node ) -> HandlerReturn
                                                          { 
                                                             if( state == State::BOL )
                                                             {
                                                                statment = node;
                                                                return {State::ANY_NODE, Impact::MOVE};
                                                             }
                                                             return { state, Impact::ERROR };
                                                          };
                                                          handlers.eol_syntax_node = [ &eol ]( const State& state, const EolSyntaxNodeSP& node ) -> HandlerReturn
                                                          {
                                                             if( state == State::ANY_NODE )
                                                             {
                                                                eol = node;
                                                                return {State::FINISH, Impact::MOVE};
                                                             }
                                                             return {state, Impact::ERROR};
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
                                                              plan.to_remove.nodes.push_back( statment );
                                                              plan.to_remove.nodes.push_back( eol );

                                                              plan.to_add.nodes.push_back( statment );
                                                              plan_or_progress = plan;
                                                          }
                                                          else {
                                                              plan_or_progress = Progress{ .readiness = iteration_result.readiness };
                                                          }
                                                          return plan_or_progress;
                                                          });
       } );
  
  // BIN_EXPR SEMICOLON
  mProductions.emplace_back(
     []( const Stack& stack, const ISyntaxNodeSP& lookahead ) -> PlanOrProgress
     {
        const size_t minimal_size = 2;
        size_t minimal_steps_number = 2;
        return find_grammar_matching_progress(stack, minimal_size, [&stack, &minimal_steps_number, &lookahead]( size_t n )->PlanOrProgress
                                                        
        {
          BinExprSyntaxNodeSP bin_expr;
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
          handlers.bin_expr_syntax_node = [ &bin_expr ]( const State& state, const BinExprSyntaxNodeSP& node ) -> HandlerReturn
          {
             if( state == State::START )
             {
                bin_expr = node;
                return { State::BIN_EXPR, Impact::MOVE };
             }
             return { state, Impact::ERROR };
          };
          handlers.semicolon_syntax_node = [ &semicolon ]( const State& state, const SemicolonSyntaxNodeSP& node ) -> HandlerReturn
          {
             if( state == State::BIN_EXPR )
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
              plan.to_remove.nodes.push_back( bin_expr );
              plan.to_remove.nodes.push_back( semicolon );

              plan.to_add.nodes.push_back( bin_expr );
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
