#include "abstract_syntax_tree.h"

#include "lexical_tokens.h"
#include "nonterminals/un_expr_syntax_node.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include "terminals/dot_syntax_node.h"
#include <gtest/gtest.h>
#include "utils/utils.h"
#include <vector>

using namespace std;

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, INDEX_BY_INT )
{
   // ARRANGE
   const auto& input = R"""(
      b[1];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, f, member_expression_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, INDEX_BY_KEY )
{
   // ARRANGE
   const auto& input = R"""(
      b["k"];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& key = std::make_shared< StringSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( key );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, f, member_expression_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, INDEX_BY_UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
      b[-1];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& minus = std::make_shared< MinusSyntaxNode >( lexical_tokens[3] );

   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
  
   std::vector< LexicalTokens::LexicalToken > un_expr_lexical_tokens{};

   const auto& un_expr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f0, un_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[5] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, un_expr, member_expression_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, INDEX_BY_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
      b[1+2];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[4] };

   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[6] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, bin_expr, member_expression_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, INDEX_BY_VARIBLE )
{
   // ARRANGE
   const auto& input = R"""(
      b[c];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name0->lexical_tokens() );
   
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, varible1, member_expression_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, DOT_ACCESS )
{
   // ARRANGE
   const auto& input = R"""(
      b.key;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   // lexical_tokens[0] = BOL, [1] = NAME "b", [2] = DOT ".", [3] = NAME "key", [4] = SEMICOLON, [5] = EOL
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& property_name = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );

   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[1], lexical_tokens[2] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, property_name, member_expression_lexical_tokens );

   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, DOT_ACCESS_CHAINED )
{
   // ARRANGE
   const auto& input = R"""(
      a.b.c;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   // [0]=BOL [1]=NAME"a" [2]=DOT [3]=NAME"b" [4]=DOT [5]=NAME"c" [6]=SEMICOLON [7]=EOL
   const auto& name_a = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible_a = std::make_shared< VaribleSyntaxNode >( name_a, name_a->lexical_tokens() );

   const auto& name_b = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );

   std::vector< LexicalTokens::LexicalToken > inner_tokens{ lexical_tokens[1], lexical_tokens[2] };
   const auto& inner_member = std::make_shared< MemberExpressionSyntaxNode >( varible_a, name_b, inner_tokens );

   const auto& name_c = std::make_shared< NameSyntaxNode >( lexical_tokens[5] );

   std::vector< LexicalTokens::LexicalToken > outer_tokens{ lexical_tokens[1], lexical_tokens[4] };
   const auto& outer_member = std::make_shared< MemberExpressionSyntaxNode >( inner_member, name_c, outer_tokens );

   AbstractSyntaxTree expected_syntax_tree { outer_member };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_MEMBER_EXPRESSION, INDEX_BY_FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
      b[foo()];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible1, std::vector<ISyntaxNodeSP>{} );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[6] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, function_call, member_expression_lexical_tokens );
  
   AbstractSyntaxTree expected_syntax_tree { member_expression };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}
