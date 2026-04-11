#include "abstract_syntax_tree.h"

#include "i_syntax_node.h"
#include "nonterminals/if_statment_syntax_node.h"
#include <gtest/gtest.h>
#include "utils/utils.h"

using namespace std;

TEST( SYNTAX_TREE_IF, F_EQUAL_F )
{
   // ARRANGE
   const auto& input = R"""(
    if(1==1)
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[4], lexical_tokens[5] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Equal, f0, f1, bin_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( bin_expr, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, F_NOT_EQUAL_F )
{
   // ARRANGE
   const auto& input = R"""(
    if(1!=1)
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[4], lexical_tokens[5] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::NotEqual, f0, f1, bin_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( bin_expr, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, MEMBER_EXPRESSION_NOT_EQUAL_MEMBER_EXPRESSION )
{
   // ARRANGE
   const auto& input = R"""(
    if(a[1]!=b[2])
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   // const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   // const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f0 = std::make_shared< FSyntaxNode >( number0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens0{ lexical_tokens[4], lexical_tokens[6] };
   const auto& member_expression0 = std::make_shared< MemberExpressionSyntaxNode >( varible0, f0, member_expression_lexical_tokens0 );
   // const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   // const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[9] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& number1 = std::make_shared< IntSyntaxNode >( lexical_tokens[11] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number1 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens1{ lexical_tokens[10], lexical_tokens[12] };
   const auto& member_expression1 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f1, member_expression_lexical_tokens1 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[7], lexical_tokens[8] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::NotEqual, member_expression0, member_expression1, bin_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( bin_expr, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, F )
{
   // ARRANGE
   const auto& input = R"""(
    if(1)
    {
    
    };
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[5], lexical_tokens[6]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( f, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, NAME )
{
   // ARRANGE
   const auto& input = R"""(
    if(a)
    {

    };
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[5], lexical_tokens[6]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( name, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    if(1+2)
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[7], lexical_tokens[8]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( bin_expr, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    if(-1)
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   std::vector< LexicalTokens::LexicalToken > un_expr_lexical_tokens{ lexical_tokens[3] };
   const auto& un_expr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f, un_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[7], lexical_tokens[8]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( un_expr, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
    if(foo(1))
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f } );
   
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( function_call, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, IF_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(
    if(foo(1))
    {

    };
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f } );
   
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( function_call, true_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_IF, IF_WITH_ELSE_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(
    if(foo(1))
    {
    }
    else
    {
    };
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f } );
   
   std::vector< LexicalTokens::LexicalToken > true_scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& true_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, true_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > false_scope_lexical_tokens{ lexical_tokens[11], lexical_tokens[12]  };
   const auto& false_scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, false_scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > if_lexical_tokens{ lexical_tokens[1] };
   const auto& if_statment = std::make_shared< IfStatmentSyntaxNode >( function_call, true_scope, false_scope, if_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { if_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}
