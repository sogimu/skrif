#include "i_syntax_node.h"
#include "nonterminals/object_pair_syntax_node.h"
#include <gtest/gtest.h>
#include "utils/utils.h"
#include <vector>

using namespace std;

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ZERO_VARIBLE )
{
   // ARRANGE
   const auto& input = R"""(
    foo();
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{} );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_VARIBLE )
{
   // ARRANGE
   const auto& input = R"""(
    foo(a);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ varible1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_TWO_VARIBLIES )
{
   // ARRANGE
   const auto& input = R"""(
    foo(a,b);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& name2 = std::make_shared< NameSyntaxNode >( lexical_tokens[5] );
   const auto& varible2 = std::make_shared< VaribleSyntaxNode >( name2, name2->lexical_tokens() );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ varible1, varible2 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
    foo([]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[3], lexical_tokens[4]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector<ISyntaxNodeSP>{}, array_lexical_tokens );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ array } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
    foo({});
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[3], lexical_tokens[4]
   };
   const auto& object = std::make_shared< ObjectSyntaxNode >( std::vector<ObjectPairSyntaxNodeSP>{}, object_lexical_tokens );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ object } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_F )
{
   // ARRANGE
   const auto& input = R"""(
    foo(1);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_TWO_F )
{
   // ARRANGE
   const auto& input = R"""(
    foo(1,2);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( number0 );
   const auto& number1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number1 );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f0, f1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    foo(1+2);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& plus = std::make_shared< PlusSyntaxNode >( lexical_tokens[4] );
   
   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ bin_expr } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_TWO_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    foo(1+2,3+4);
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
  
   std::vector< LexicalTokens::LexicalToken > bin_expr0_lexical_tokens{ lexical_tokens[4] };
   const auto& bin_expr0 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr0_lexical_tokens );
   const auto& d2 = std::make_shared< IntSyntaxNode >( lexical_tokens[7] );
   const auto& f2 = std::make_shared< FSyntaxNode >( d2 );

   const auto& d3 = std::make_shared< IntSyntaxNode >( lexical_tokens[9] );
   const auto& f3 = std::make_shared< FSyntaxNode >( d3 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr1_lexical_tokens{ lexical_tokens[8] };
   const auto& bin_expr1 = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f2, f3, bin_expr1_lexical_tokens );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ bin_expr0, bin_expr1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    foo(-1);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   const auto& unexpr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[3] } );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ unexpr } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_TWO_UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    foo(-1,-2);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );
   const auto& unexpr0 = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f0, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[3] } );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[7] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
   const auto& unexpr1 = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f1, std::vector<LexicalTokens::LexicalToken>{ lexical_tokens[6] } );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ unexpr0, unexpr1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_MEMBER_EXPRESSION )
{
   // ARRANGE
   const auto& input = R"""(
    foo(a["k"]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& key = std::make_shared< StringSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( key );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[4], lexical_tokens[6] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible1, f, member_expression_lexical_tokens );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ member_expression } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_TWO_MEMBER_EXPRESSION )
{
   // ARRANGE
   const auto& input = R"""(
    foo(a["k"], b["j"]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& key0 = std::make_shared< StringSyntaxNode >( lexical_tokens[5] );
   const auto& f0 = std::make_shared< FSyntaxNode >( key0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens0{ lexical_tokens[3], lexical_tokens[4], lexical_tokens[6] };
   const auto& member_expression0 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f0, member_expression_lexical_tokens0 );
   
   const auto& name2 = std::make_shared< NameSyntaxNode >( lexical_tokens[8] );
   const auto& varible2 = std::make_shared< VaribleSyntaxNode >( name2, name2->lexical_tokens() );

   const auto& key1 = std::make_shared< StringSyntaxNode >( lexical_tokens[10] );
   const auto& f1 = std::make_shared< FSyntaxNode >( key1 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens1{ lexical_tokens[8], lexical_tokens[9], lexical_tokens[11] };
   const auto& member_expression1 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f1, member_expression_lexical_tokens1 );
   
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ member_expression0, member_expression1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_ONE_FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
    foo(bar(1));
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   const auto& function_call0 = std::make_shared< FunctionCallSyntaxNode >( varible1, std::vector< ISyntaxNodeSP >{ f } );
   
   const auto& function_call1 = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ function_call0 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call1 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_TWO_FUNCTION_CALLS )
{
   // ARRANGE
   const auto& input = R"""(
    foo( bar(1), zigbee(2) );
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] ); 
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f0 = std::make_shared< FSyntaxNode >( number0 );
   const auto& function_call0 = std::make_shared< FunctionCallSyntaxNode >( varible1, std::vector< ISyntaxNodeSP >{ f0 } );
   
   const auto& name2 = std::make_shared< NameSyntaxNode >( lexical_tokens[8] ); 
   const auto& varible2 = std::make_shared< VaribleSyntaxNode >( name2, name2->lexical_tokens() );
   const auto& number1 = std::make_shared< IntSyntaxNode >( lexical_tokens[10] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number1 );
   const auto& function_call1 = std::make_shared< FunctionCallSyntaxNode >( varible2, std::vector< ISyntaxNodeSP >{ f1 } );
   
   const auto& function_call2 = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ function_call0, function_call1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_MEMBER_EXPRESSION )
{
   // ARRANGE
   const auto& input = R"""(
    m["k"]();
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
   
   const auto& function_call2 = std::make_shared< FunctionCallSyntaxNode >( member_expression, std::vector< ISyntaxNodeSP >{} );
   
   AbstractSyntaxTree expected_syntax_tree { function_call2 };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_MEMBER_EXPRESSION_WHEN_ARGUMENTS_ONE_MEMBER_EXPRESSION_OF_OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
    m["k"](a["k"]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& key0 = std::make_shared< StringSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( key0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens0{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression0 = std::make_shared< MemberExpressionSyntaxNode >( varible0, f0, member_expression_lexical_tokens0 );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& key1 = std::make_shared< StringSyntaxNode >( lexical_tokens[8] );
   const auto& f1 = std::make_shared< FSyntaxNode >( key1 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens1{ lexical_tokens[7], lexical_tokens[9] };
   const auto& member_expression1 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f1, member_expression_lexical_tokens1 );
   
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( member_expression0, std::vector< ISyntaxNodeSP >{ member_expression1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_MEMBER_EXPRESSION_WHEN_ARGUMENTS_TWO_MEMBER_EXPRESSION_OF_OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
    m["k"](a["k"], b["j"]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& key0 = std::make_shared< StringSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( key0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens0{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression0 = std::make_shared< MemberExpressionSyntaxNode >( varible0, f0, member_expression_lexical_tokens0 );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& key1 = std::make_shared< StringSyntaxNode >( lexical_tokens[8] );
   const auto& f1 = std::make_shared< FSyntaxNode >( key1 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens1{ lexical_tokens[7], lexical_tokens[9] };
   const auto& member_expression1 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f1, member_expression_lexical_tokens1 );
   
   const auto& name2 = std::make_shared< NameSyntaxNode >( lexical_tokens[11] );
   const auto& varible2 = std::make_shared< VaribleSyntaxNode >( name2, name2->lexical_tokens() );

   const auto& key2 = std::make_shared< StringSyntaxNode >( lexical_tokens[13] );
   const auto& f2 = std::make_shared< FSyntaxNode >( key2 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens2{ lexical_tokens[12], lexical_tokens[14] };
   const auto& member_expression2 = std::make_shared< MemberExpressionSyntaxNode >( varible2, f2, member_expression_lexical_tokens2 );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( member_expression0, std::vector< ISyntaxNodeSP >{ member_expression1, member_expression2 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_MEMBER_EXPRESSION_WHEN_ARGUMENTS_ONE_MEMBER_EXPRESSION_OF_ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
    m["k"](a[0]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& key0 = std::make_shared< StringSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( key0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens0{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression0 = std::make_shared< MemberExpressionSyntaxNode >( varible0, f0, member_expression_lexical_tokens0 );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[8] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens1{ lexical_tokens[7], lexical_tokens[9] };
   const auto& member_expression1 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f1, member_expression_lexical_tokens1 );
   
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( member_expression0, std::vector< ISyntaxNodeSP >{ member_expression1 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_FUNCTION_CALL, FROM_MEMBER_EXPRESSION_WHEN_ARGUMENTS_TWO_MEMBER_EXPRESSION_OF_ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
    m["k"](a[0], b[1]);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& key0 = std::make_shared< StringSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( key0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens0{ lexical_tokens[1], lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression0 = std::make_shared< MemberExpressionSyntaxNode >( varible0, f0, member_expression_lexical_tokens0 );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );

   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[8] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number0 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens1{ lexical_tokens[6], lexical_tokens[7], lexical_tokens[9] };
   const auto& member_expression1 = std::make_shared< MemberExpressionSyntaxNode >( varible1, f1, member_expression_lexical_tokens1 );
   
   const auto& name2 = std::make_shared< NameSyntaxNode >( lexical_tokens[11] );
   const auto& varible2 = std::make_shared< VaribleSyntaxNode >( name2, name2->lexical_tokens() );

   const auto& number1 = std::make_shared< IntSyntaxNode >( lexical_tokens[13] );
   const auto& f2 = std::make_shared< FSyntaxNode >( number1 );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens2{ lexical_tokens[11], lexical_tokens[12], lexical_tokens[14] };
   const auto& member_expression2 = std::make_shared< MemberExpressionSyntaxNode >( varible2, f2, member_expression_lexical_tokens2 );
   
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( member_expression0, std::vector< ISyntaxNodeSP >{ member_expression1, member_expression2 } );
   
   AbstractSyntaxTree expected_syntax_tree { function_call };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

