#include "abstract_syntax_tree.h"

#include "nonterminals/function_call_syntax_node.h"
#include "nonterminals/un_expr_syntax_node.h"
#include "terminals/asterisk_syntax_node.h"
#include "terminals/equal_syntax_node.h"
#include "terminals/open_circle_bracket_syntax_node.h"
#include "terminals/slash_syntax_node.h"
#include <gtest/gtest.h>
#include "utils/utils.h"
#include <memory>

#include <syntax_nodes/nonterminals/statment_syntax_node.h>
#include <syntax_nodes/terminals/double_syntax_node.h>
#include <lexical_tokens/lexical_tokens.h>

using namespace std;

TEST( SYNTAX_TREE_ARITHMETIC, INT_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(1;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& i = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f = std::make_shared< FSyntaxNode >( i );
   AbstractSyntaxTree expected_syntax_tree { f };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, DOUBLE_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(1.0;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d = std::make_shared< DoubleSyntaxNode >( lexical_tokens[1] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   AbstractSyntaxTree expected_syntax_tree { f };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, MINUS_DOUBLE_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(-1.0;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d = std::make_shared< DoubleSyntaxNode >( lexical_tokens[2] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   const auto& unexpr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[1] } );
   AbstractSyntaxTree expected_syntax_tree { unexpr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, MINUS_INT_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(-1;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   const auto& unexpr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[1] } );
   AbstractSyntaxTree expected_syntax_tree { unexpr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, UN_EXPR_LAST )
{
   // ARRANGE
   const auto& input = R"""(1--2;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   const auto& unexpr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f1, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[3] } );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, f0, unexpr, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, UN_EXPR_NOT_LAST )
{
   // ARRANGE
   const auto& input = R"""(-2-1;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   const auto& unexpr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f0, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[1] } );
   
   const auto& n1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( n1 );

   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[3] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, unexpr, f1, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, OPEN_BRACKET_NUMBER_MINUS_NUMBER_DIVIDE_NUMBER_CLOSE_BRACKET )
{
   // ARRANGE
   const auto& input = R"""((5-2/3);)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );

   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[5] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Division, f1, f2, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[3] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, f0, bin_expr0, bin_expr1_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, OPEN_BRACKET_NUMBER_MULTIPLY_OPEN_BRACKET_NUMBER_CLOSE_BRACKET_CLOSE_BRACKET )
{
   // ARRANGE
   const auto& input = R"""((1*(8));)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[3] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Multiply, f0, f1, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, OPEN_BRACKET_NUMBER_CLOSE_BRACKET )
{
   // ARRANGE
   const auto& input = R"""((1);)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   AbstractSyntaxTree expected_syntax_tree { f };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, ONE_PLUS )
{
   // ARRANGE
   const auto& input = R"""(1+2;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_F_EQUAL_F )
{
   // ARRANGE
   const auto& input = R"""(1==2;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2], lexical_tokens[3] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Equal, f0, f1, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_F_EQUAL_NAME )
{
   // ARRANGE
   const auto& input = R"""(1==a;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[4] );
   const auto& varible = std::make_shared< VaribleSyntaxNode >( name, name->lexical_tokens() );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2], lexical_tokens[3] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Equal, f0, varible, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_F_LESS_NAME )
{
   // ARRANGE
   const auto& input = R"""(1<a;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible = std::make_shared< VaribleSyntaxNode >( name, name->lexical_tokens() );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Less, f0, varible, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_F_MORE_NAME )
{
   // ARRANGE
   const auto& input = R"""(1>a;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible = std::make_shared< VaribleSyntaxNode >( name, name->lexical_tokens() );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::More, f0, varible, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_BIN_EXPR_EQUAL_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(1+2==3+4;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& d3 = std::make_shared< IntSyntaxNode >( lexical_tokens[8] );
   const auto& f3 = std::make_shared< FSyntaxNode >( d3 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[7] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f2, f3, bin_expr1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr2_lexical_tokens{ lexical_tokens[4], lexical_tokens[5] };
   const auto& bin_expr2 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Equal, bin_expr0, bin_expr1, bin_expr2_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_BIN_EXPR_MORE_EQUAL_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(1+2>=3+4;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& d3 = std::make_shared< IntSyntaxNode >( lexical_tokens[8] );
   const auto& f3 = std::make_shared< FSyntaxNode >( d3 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[7] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f2, f3, bin_expr1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr2_lexical_tokens{ lexical_tokens[4], lexical_tokens[5] };
   const auto& bin_expr2 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::MoreEqual, bin_expr0, bin_expr1, bin_expr2_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_BIN_EXPR_LESS_EQUAL_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(1+2<=3+4;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& d3 = std::make_shared< IntSyntaxNode >( lexical_tokens[8] );
   const auto& f3 = std::make_shared< FSyntaxNode >( d3 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[7] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f2, f3, bin_expr1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr2_lexical_tokens{ lexical_tokens[4], lexical_tokens[5] };
   const auto& bin_expr2 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::LessEqual, bin_expr0, bin_expr1, bin_expr2_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_BIN_EXPR_LESS_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(1+2<3+4;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& d3 = std::make_shared< IntSyntaxNode >( lexical_tokens[7] );
   const auto& f3 = std::make_shared< FSyntaxNode >( d3 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[6] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f2, f3, bin_expr1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr2_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr2 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Less, bin_expr0, bin_expr1, bin_expr2_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CONDITION_BIN_EXPR_MORE_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(1+2>3+4;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& d3 = std::make_shared< IntSyntaxNode >( lexical_tokens[7] );
   const auto& f3 = std::make_shared< FSyntaxNode >( d3 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[6] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f2, f3, bin_expr1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr2_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr2 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::More, bin_expr0, bin_expr1, bin_expr2_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, F_PLUS_VARIBLE )
{
   // ARRANGE
   const auto& input = R"""(1+a;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible = std::make_shared< VaribleSyntaxNode >( name, name->lexical_tokens() );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, varible, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, F_PLUS_FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(1+foo(2);)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f1 } );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, function_call, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, ONE_DIVISION )
{
   // ARRANGE
   const auto& input = R"""(6/2;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Division, f0, f1, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, MANY_PLUSs )
{
   // ARRANGE
   const auto& input = R"""(1+2+3;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f1, f2, bin_expr0_lexical_tokens );

   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, bin_expr0, bin_expr1_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, MANY_MINUSES )
{
   // ARRANGE
   const auto& input = R"""(1-2-3;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, f1, f2, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, f0, bin_expr0, bin_expr1_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, ADVANCE_EXPRESSION0 )
{
   // ARRANGE
   const auto& input = R"""(1+2*3;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Multiply, f1, f2, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, bin_expr0, bin_expr1_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, ADVANCE_EXPRESSION1 )
{
   // ARRANGE
   const auto& input = R"""(x-1/x;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
  
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[5] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Division, f, varible1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, varible0, bin_expr0, bin_expr1_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, ADVANCE_EXPRESSION2 )
{
   // ARRANGE
   const auto& input = R"""(1-x*a-c;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
  
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[5] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name0->lexical_tokens() );
   
   const auto& name2 = std::make_shared< NameSyntaxNode >( lexical_tokens[7] );
   const auto& varible2 = std::make_shared< VaribleSyntaxNode >( name2, name0->lexical_tokens() );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Multiply, varible0, varible1, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[6] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, bin_expr0, varible2, bin_expr1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr2_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr2 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, f, bin_expr1, bin_expr2_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CIRCLE_CASE0 )
{
   // ARRANGE
   const auto& input = R"""(1+(2*3);)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& open_circle_bracket = std::make_shared< OpenCircleBracketSyntaxNode >( lexical_tokens[3] );
   
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& close_circle_bracket = std::make_shared< CloseCircleBracketSyntaxNode >( lexical_tokens[7] );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[5] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Multiply, f1, f2, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, bin_expr0, bin_expr1_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, CIRCLE_CASE1 )
{
   // ARRANGE
   const auto& input = R"""(1+(2+3);)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& open_circle_bracket = std::make_shared< OpenCircleBracketSyntaxNode >( lexical_tokens[3] );
   
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );
   
   const auto& close_circle_bracket = std::make_shared< CloseCircleBracketSyntaxNode >( lexical_tokens[7] );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[5] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f1, f2, bin_expr0_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, bin_expr0, bin_expr1_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { bin_expr1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARITHMETIC, ONE_MINUS )
{
   // ARRANGE
   const auto& input = R"""(1-2;)""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[1] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[2] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Substruction, f0, f1, bin_expr_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { bin_expr };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

