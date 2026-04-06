#include "abstract_syntax_tree.h"

#include "lexical_tokens.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include <gtest/gtest.h>
#include "utils/utils.h"

using namespace std;

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, F )
{
   // ARRANGE
   const auto& input = R"""(
      a=1;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name, f, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
      a=1+2;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[4] };

   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name, bin_expr, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
      a=-1;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f = std::make_shared< FSyntaxNode >( d );

   std::vector< LexicalTokens::LexicalToken > un_expr_lexical_tokens{ lexical_tokens[3] };

   const auto& un_expr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f, un_expr_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name, un_expr, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, NAME )
{
   // ARRANGE
   const auto& input = R"""(
      a=b;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, varible1, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, FROM_MEMBER_EXPRESSION_OF_ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
      a=b[1];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[3], lexical_tokens[4], lexical_tokens[6] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible1, f, member_expression_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, member_expression, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, TO_MEMBER_EXPRESSION_OF_OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
      b["k"]=a;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name0->lexical_tokens() );
   const auto& key = std::make_shared< StringSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( key );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, f, member_expression_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( member_expression, varible1, lexical_tokens[5] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, TO_MEMBER_EXPRESSION_OF_ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
      b[1]=a;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name0->lexical_tokens() );
   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[2], lexical_tokens[4] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible0, f, member_expression_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( member_expression, varible1, lexical_tokens[5] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, FROM_MEMBER_EXPRESSION_OF_OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
      a=b["k"];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name0->lexical_tokens() );
   const auto& key = std::make_shared< StringSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( key );
   std::vector< LexicalTokens::LexicalToken > member_expression_lexical_tokens{ lexical_tokens[3], lexical_tokens[4], lexical_tokens[6] };
   const auto& member_expression = std::make_shared< MemberExpressionSyntaxNode >( varible1, f, member_expression_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, member_expression, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
      a=foo(1);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible1, std::vector< ISyntaxNodeSP >{ f } );

   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, function_call, lexical_tokens[2] );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
      a=[];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[3], lexical_tokens[4]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{}, array_lexical_tokens );

   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, array, lexical_tokens[2], VaribleAssigmentStatmentSyntaxNode::Context::GLOBAL );
   
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_GLOBAL_VARIBLE_ASSIGN, OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
      a={};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[1] );

   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[3], lexical_tokens[4]
   };
   const auto& object = std::make_shared< ObjectSyntaxNode >( std::vector< ObjectPairSyntaxNodeSP >{}, object_lexical_tokens );

   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, object, lexical_tokens[2], VaribleAssigmentStatmentSyntaxNode::Context::GLOBAL );
   
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, F )
{
   // ARRANGE
   const auto& input = R"""(
      var a=1;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );

   const auto& number = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f = std::make_shared< FSyntaxNode >( number );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name, f, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
      var a=1+2;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );

   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[5] };

   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name, bin_expr, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
      var a=-1;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );
   const auto& varible = std::make_shared< VaribleSyntaxNode >( name, name->lexical_tokens() );

   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[5] );
   const auto& f = std::make_shared< FSyntaxNode >( d );

   std::vector< LexicalTokens::LexicalToken > un_expr_lexical_tokens{ lexical_tokens[4] };

   const auto& un_expr = std::make_shared< UnExprSyntaxNode >( UnExprSyntaxNode::Type::Negation, f, un_expr_lexical_tokens );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name, un_expr, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, NAME )
{
   // ARRANGE
   const auto& input = R"""(
      var a=b;
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[4] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
  
   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, varible1, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
      var a=foo(1);
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );

   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[4] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{ f } );

   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, function_call, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, ARRAY )
{
   // ARRANGE
   const auto& input = R"""(
      var a=[];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );

   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[4], lexical_tokens[5]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{}, array_lexical_tokens );

   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, array, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_LOCAL_VARIBLE_ASSIGN, OBJECT )
{
   // ARRANGE
   const auto& input = R"""(
      var a={};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );

   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[4], lexical_tokens[5]
   };
   const auto& object = std::make_shared< ObjectSyntaxNode >( std::vector< ObjectPairSyntaxNodeSP >{}, object_lexical_tokens );

   const auto& varible_assigment = std::make_shared< VaribleAssigmentStatmentSyntaxNode >( name0, object, lexical_tokens[3], VaribleAssigmentStatmentSyntaxNode::Context::LOCAL );
   
   AbstractSyntaxTree expected_syntax_tree { varible_assigment };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

