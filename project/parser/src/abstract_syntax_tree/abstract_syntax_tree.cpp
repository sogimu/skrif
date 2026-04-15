#include "abstract_syntax_tree.h"

#include "syntax_exception.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <vector>
#include "syntax_exception.hpp"
#include "enums.h"
#include "grammars/un_expr_grammar.h"
#include "grammars/bin_expr_grammar.h"
#include "grammars/circle_brackets_grammar.h"
#include "grammars/member_expression_grammar.h"
#include "grammars/f_grammar.h"
#include "grammars/if_statment_grammar.h"
#include "grammars/while_statment_grammar.h"
#include "grammars/function_statment_grammar.h"
#include "grammars/function_call_grammar.h"
#include "grammars/print_statment_grammar.h"
#include "grammars/scope_grammar.h"
#include "grammars/array_grammar.h"
#include "grammars/object_grammar.h"
#include "grammars/varible_assigment_grammar.h"
#include "grammars/return_statment_grammar.h"
#include "i_grammar.h"
#include "i_syntax_node.h"
#include "lexical_tokens.h"
#include "utils.h"
#include "nonterminals/function_statment_syntax_node.h"
#include "nonterminals/statment_syntax_node.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include "nonterminals/varible_syntax_node.h"
#include "terminals/asterisk_syntax_node.h"
#include "terminals/return_syntax_node.h"
#include "terminals/slash_syntax_node.h"
#include "syntax_nodes/terminals/bol_syntax_node.h"
#include "syntax_nodes/terminals/close_curly_bracket_syntax_node.h"
#include "syntax_nodes/terminals/eol_syntax_node.h"
#include "syntax_nodes/terminals/if_syntax_node.h"
#include "syntax_nodes/terminals/else_syntax_node.h"
#include "syntax_nodes/terminals/while_syntax_node.h"
#include "syntax_nodes/terminals/minus_syntax_node.h"
#include "syntax_nodes/terminals/name_syntax_node.h"
#include "syntax_nodes/terminals/comma_syntax_node.h"
#include "syntax_nodes/terminals/double_syntax_node.h"
#include "syntax_nodes/terminals/open_curly_bracket_syntax_node.h"
#include "syntax_nodes/nonterminals/bin_expr_syntax_node.h"
#include "syntax_nodes/terminals/plus_syntax_node.h"
#include "syntax_nodes/terminals/semicolon_syntax_node.h"
#include "syntax_nodes/terminals/dot_syntax_node.h"
#include "syntax_nodes/terminals/var_syntax_node.h"
#include "syntax_nodes/terminals/string_syntax_node.h"

ISyntaxNodeSP createSyntaxNodeFromToken( const LexicalTokens::LexicalToken& lexical_token )
{
  ISyntaxNodeSP syntax_node;
  switch( lexical_token.type )
  {
  case Token_Type::BOL:
  {
     syntax_node = std::make_shared< BolSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::EOL:
  {
     syntax_node = std::make_shared< EolSyntaxNode >( lexical_token );
  };
  break;

  case Token_Type::PLUS:
  {
     syntax_node = std::make_shared< PlusSyntaxNode >( lexical_token );
  };
  break;

  case Token_Type::MINUS:
  {
     syntax_node = std::make_shared< MinusSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::ASTERISK:
  {
     syntax_node = std::make_shared< AsteriskSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::SLASH:
  {
     syntax_node = std::make_shared< SlashSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::DOUBLE:
  {
     syntax_node = std::make_shared< DoubleSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::INT:
  {
     syntax_node = std::make_shared< IntSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::EQUAL:
  {
     syntax_node = std::make_shared< EqualSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::NOT:
  {
     syntax_node = std::make_shared< NotSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::LESS:
  {
     syntax_node = std::make_shared< LessSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::MORE:
  {
     syntax_node = std::make_shared< MoreSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::SEMICOLON:
  {
     syntax_node = std::make_shared< SemicolonSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::COLON:
  {
     syntax_node = std::make_shared< ColonSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::VAR:
  {
     syntax_node = std::make_shared< VarSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::OPEN_CURLY_BRACKET:
  {
     syntax_node = std::make_shared< OpenCurlyBracketSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::CLOSE_CURLY_BRACKET:
  {
     syntax_node = std::make_shared< CloseCurlyBracketSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::OPEN_CIRCLE_BRACKET:
  {
     syntax_node = std::make_shared< OpenCircleBracketSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::CLOSE_CIRCLE_BRACKET:
  {
     syntax_node = std::make_shared< CloseCircleBracketSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::OPEN_SQUARE_BRACKET:
  {
     syntax_node = std::make_shared< OpenSquareBracketSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::CLOSE_SQUARE_BRACKET:
  {
     syntax_node = std::make_shared< CloseSquareBracketSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::PRINT:
  {
     syntax_node = std::make_shared< PrintSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::IF:
  {
     syntax_node = std::make_shared< IfSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::ELSE:
  {
     syntax_node = std::make_shared< ElseSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::WHILE:
  {
     syntax_node = std::make_shared< WhileSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::FUNCTION:
  {
     syntax_node = std::make_shared< FunctionSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::NAME:
  {
     syntax_node = std::make_shared< NameSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::COMMA:
  {
     syntax_node = std::make_shared< CommaSyntaxNode >( lexical_token);
  };
  break;
  case Token_Type::RETURN:
  {
     syntax_node = std::make_shared< ReturnSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::STRING:
  {
     syntax_node = std::make_shared< StringSyntaxNode >( lexical_token );
  };
  break;
  case Token_Type::DOT:
  {
     syntax_node = std::make_shared< DotSyntaxNode >( lexical_token );
  };
  break;
  default:
  {
  }
  }
  return syntax_node;
}

AbstractSyntaxTree::AbstractSyntaxTree( const ISyntaxNodeSP& root )
  : mRoot{ root }
{
}

AbstractSyntaxTree::AbstractSyntaxTree( const LexicalTokens& lexical_tokens )
 {
    std::vector< IGrammarSP > grammars{
       std::make_shared< FunctionCall >(),
       std::make_shared< Print >(),
       std::make_shared< Return >(),
       std::make_shared< CircleBrackets >(),
       std::make_shared< MemberExpression >(),
       std::make_shared< F >(),
       std::make_shared< BinExpr >(),
       std::make_shared< UnExpr >(),
       std::make_shared< VaribleAssigment >(),
       std::make_shared< Array >(),
       std::make_shared< Object >(),
       std::make_shared< Scope >(),
       std::make_shared< If >(),
       std::make_shared< While >(),
       std::make_shared< FunctionStatment >()
    };

    // Stack stack;
    const auto& tokens = lexical_tokens.list();
    auto it = tokens.begin();
    do
    {
       std::optional<IGrammar::Progress> best_progress_opt;
       ISyntaxNodeSP lookahead_node;
       if( auto next_it = std::next( it ); next_it != tokens.end() )
       {
         lookahead_node = createSyntaxNodeFromToken( *next_it );
       }
       const auto& token = *it;
       const auto& node = createSyntaxNodeFromToken( token );
       mStack.emplace_back( node );

       bool try_reduce = true;
       while( try_reduce )
       {
          try_reduce = false;
          for( const auto& grammar : grammars )
          {
            const auto& try_reduce_result = grammar->TryReduce( mStack, lookahead_node );
            
            std::optional<IGrammar::Plan> plan_opt;
            std::visit([&try_reduce, &best_progress_opt, &plan_opt, this](auto&& arg)
            {
              using T = std::decay_t<decltype(arg)>;
              if constexpr (std::is_same_v<T, IGrammar::Progress>)
              {
                  if( !best_progress_opt || arg.readiness > best_progress_opt->readiness )
                      best_progress_opt = arg;
              } 
              else if constexpr (std::is_same_v<T, IGrammar::Plan>) 
              {
                  plan_opt = arg;
              }          
            }, try_reduce_result);
          
            if( plan_opt )
            {
              const auto& plan = plan_opt.value();

              for( size_t i = 0; i < plan.to_remove.nodes.size(); ++i )
                mStack.pop_back();

              for( const auto& d : plan.to_add.nodes )
                mStack.push_back( d );
              
              try_reduce = true;
              break;
            }
          }
       }
       
       if( best_progress_opt && best_progress_opt->readiness == 0.0 )
          if( mStack.size() > 1 )
              throw SyntaxException( "Stack has more that on node and no one grammars may reduce current stack. It's seems that there is syntax error in program!", mStack );
       
       ++it;
    }
    while( it != tokens.end() );

    if( mStack.size() > 1 )
       throw std::runtime_error( "Not all nodes reduced" );

    if( mStack.size() == 0 )
       throw std::runtime_error( "No final reduced node" );

    mRoot = mStack.front();
 }

ISyntaxNodeSP AbstractSyntaxTree::root() const
{
  return mRoot;
}


bool AbstractSyntaxTree::operator==( const AbstractSyntaxTree& syntax_tree ) const
{
return ::equal( syntax_tree.mRoot, mRoot );
}

std::string AbstractSyntaxTree::to_string() const
{
  return ::to_string( mRoot );
}
