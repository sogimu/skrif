#include <gtest/gtest.h>
#include "utils/utils.h"
#include "nonterminals/array_syntax_node.h"

using namespace std;

TEST( SYNTAX_TREE_ARRAY, FROM_TWO_INT )
{
   // ARRANGE
   const auto& input = R"""(
    [1,2,3.4];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& number0 = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f0 = std::make_shared< FSyntaxNode >( number0 );
   const auto& number1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( number1 );
   const auto& number2 = std::make_shared< DoubleSyntaxNode >( lexical_tokens[6] );
   const auto& f2 = std::make_shared< FSyntaxNode >( number2 );
   
   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[7]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{ f0, f1, f2 }, array_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { array };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARRAY, FROM_ONE_NAME )
{
   // ARRANGE
   const auto& input = R"""(
    [a];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   
   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[3]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{ varible0 }, array_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { array };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARRAY, FROM_ONE_FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
    [foo()];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& function_call = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{} );
   
   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[3]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{ function_call }, array_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { array };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARRAY, FROM_TWO_FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
    [foo(), foo()];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name0 = std::make_shared< NameSyntaxNode >( lexical_tokens[2] );
   const auto& varible0 = std::make_shared< VaribleSyntaxNode >( name0, name0->lexical_tokens() );
   const auto& function_call0 = std::make_shared< FunctionCallSyntaxNode >( varible0, std::vector< ISyntaxNodeSP >{} );
   
   const auto& name1 = std::make_shared< NameSyntaxNode >( lexical_tokens[6] );
   const auto& varible1 = std::make_shared< VaribleSyntaxNode >( name1, name1->lexical_tokens() );
   const auto& function_call1 = std::make_shared< FunctionCallSyntaxNode >( varible1, std::vector< ISyntaxNodeSP >{} );

   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[9]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{ function_call0, function_call1 }, array_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { array };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}


TEST( SYNTAX_TREE_ARRAY, FROM_BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    [1+2];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d0 = std::make_shared< IntSyntaxNode >( lexical_tokens[2] );
   const auto& f0 = std::make_shared< FSyntaxNode >( d0 );

   const auto& d1 = std::make_shared< IntSyntaxNode >( lexical_tokens[4] );
   const auto& f1 = std::make_shared< FSyntaxNode >( d1 );
  
   std::vector< LexicalTokens::LexicalToken > bin_expr_lexical_tokens{ lexical_tokens[3] };
   const auto& bin_expr = std::make_shared< BinExprSyntaxNode >( BinExprSyntaxNode::Type::Addition, f0, f1, bin_expr_lexical_tokens );
   
   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[5]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{ bin_expr }, array_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { array };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_ARRAY, EMPTY )
{
   // ARRANGE
   const auto& input = R"""(
    [];
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   std::vector< LexicalTokens::LexicalToken > array_lexical_tokens
   {
      lexical_tokens[1], lexical_tokens[2]
   };
   const auto& array = std::make_shared< ArraySyntaxNode >( std::vector< ISyntaxNodeSP >{}, array_lexical_tokens );
   
   AbstractSyntaxTree expected_syntax_tree { array };
   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}
