#include <gtest/gtest.h>
#include "utils/utils.h"
#include "nonterminals/object_pair_syntax_node.h"
#include "nonterminals/object_syntax_node.h"
#include "terminals/string_syntax_node.h"

using namespace std;

TEST( SYNTAX_TREE_OBJECT, FROM_TWO_INT )
{
   // ARRANGE
   const auto& input = R"""(
    {"key": 12, "key1": 42};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& string0 = std::make_shared< StringSyntaxNode >( lexical_tokens[2] );
   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f0 = std::make_shared< FSyntaxNode >( number0 );
   std::vector< LexicalTokens::LexicalToken > pair0_lexical_tokens
   {
      lexical_tokens[3]
   };
   const auto& pair0 = std::make_shared< ObjectPairSyntaxNode >( string0, f0, pair0_lexical_tokens );
  
   const auto& string1 = std::make_shared< StringSyntaxNode >( lexical_tokens[6] );
   const auto& number1 = std::make_shared< IntSyntaxNode >( lexical_tokens[8] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number1 );
   std::vector< LexicalTokens::LexicalToken > pair1_lexical_tokens
   {
      lexical_tokens[7]
   };
   const auto& pair1 = std::make_shared< ObjectPairSyntaxNode >( string1, f1, pair1_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[9]
   };
   std::vector< ObjectPairSyntaxNodeSP > pairs{ pair0, pair1 };
   const auto& object = std::make_shared< ObjectSyntaxNode >( pairs, object_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { object };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_OBJECT, FROM_ONE_INT )
{
   // ARRANGE
   const auto& input = R"""(
    {"key": 12};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& string0 = std::make_shared< StringSyntaxNode >( lexical_tokens[2] );
   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f0 = std::make_shared< FSyntaxNode >( number0 );
   std::vector< LexicalTokens::LexicalToken > pair0_lexical_tokens
   {
      lexical_tokens[3]
   };
   const auto& pair0 = std::make_shared< ObjectPairSyntaxNode >( string0, f0, pair0_lexical_tokens );
  
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[5]
   };
   std::vector< ObjectPairSyntaxNodeSP > pairs{ pair0 };
   const auto& object = std::make_shared< ObjectSyntaxNode >( pairs, object_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { object };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_OBJECT, FROM_FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
    {"key": foo()};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& string0 = std::make_shared< StringSyntaxNode >( lexical_tokens[2] );
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[4] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{} );
   std::vector< LexicalTokens::LexicalToken > pair0_lexical_tokens
   {
      lexical_tokens[3]
   };
   const auto& pair0 = std::make_shared< ObjectPairSyntaxNode >( string0, function_call, pair0_lexical_tokens );
  
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[5]
   };
   std::vector< ObjectPairSyntaxNodeSP > pairs{ pair0 };
   const auto& object = std::make_shared< ObjectSyntaxNode >( pairs, object_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { object };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_OBJECT, FROM_VARIBLE )
{
   // ARRANGE
   const auto& input = R"""(
    {"key": a};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& string0 = std::make_shared< StringSyntaxNode >( lexical_tokens[2] );
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[4] );
   const auto& varible = std::make_shared< VaribleSyntaxNode >( name, name->lexical_tokens() );
   std::vector< LexicalTokens::LexicalToken > pair0_lexical_tokens
   {
      lexical_tokens[3]
   };
   const auto& pair0 = std::make_shared< ObjectPairSyntaxNode >( string0, varible, pair0_lexical_tokens );
  
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[5]
   };
   std::vector< ObjectPairSyntaxNodeSP > pairs{ pair0 };
   const auto& object = std::make_shared< ObjectSyntaxNode >( pairs, object_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { object };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_OBJECT, FROM_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    {"key": 1+2};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& string0 = std::make_shared< StringSyntaxNode >( lexical_tokens[2] );
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[6] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[5] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > pair0_lexical_tokens
   {
      lexical_tokens[3]
   };
   const auto& pair0 = std::make_shared< ObjectPairSyntaxNode >( string0, bin_expr, pair0_lexical_tokens );
  
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[5]
   };
   std::vector< ObjectPairSyntaxNodeSP > pairs{ pair0 };
   const auto& object = std::make_shared< ObjectSyntaxNode >( pairs, object_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { object };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_OBJECT, EMPTY )
{
   // ARRANGE
   const auto& input = R"""(
    {};
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   std::vector< LexicalTokens::LexicalToken > object_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[2]
   };
   const auto& object = std::make_shared< ObjectSyntaxNode >( std::vector< ObjectPairSyntaxNodeSP >{}, object_lexical_tokens );

   AbstractSyntaxTree expected_syntax_tree { object };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}
