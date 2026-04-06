#include "abstract_syntax_tree.h"

#include <gtest/gtest.h>
#include "utils/utils.h"

using namespace std;

TEST( SYNTAX_TREE_WHILE, F_EQUAL_F )
{
   // ARRANGE
   const auto& input = R"""(
    while(1==1)
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
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( bin_expr, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_WHILE, F )
{
   // ARRANGE
   const auto& input = R"""(
    while(1)
    {

    }
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& d = std::make_shared< IntSyntaxNode >( lexical_tokens[3] );
   const auto& f = std::make_shared< FSyntaxNode >( d );
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[5], lexical_tokens[6]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( f, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_WHILE, NAME )
{
   // ARRANGE
   const auto& input = R"""(
    while(a)
    {

    };
   )""";

   // ACT
   const auto& lexical_tokens = LexicalTokens( input );
   const auto& syntax_tree = AbstractSyntaxTree( lexical_tokens );

   // ASSERT
   const auto& name = std::make_shared< NameSyntaxNode >( lexical_tokens[3] );
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[5], lexical_tokens[6]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( name, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_WHILE, BIN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    while(1+2)
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
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[7], lexical_tokens[8]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( bin_expr, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_WHILE, UN_EXPR )
{
   // ARRANGE
   const auto& input = R"""(
    while(-1)
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
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[7], lexical_tokens[8]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( un_expr, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_WHILE, FUNCTION_CALL )
{
   // ARRANGE
   const auto& input = R"""(
    while(foo(1))
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
   
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( function_call, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}

TEST( SYNTAX_TREE_WHILE, IF_SEMICOLON )
{
   // ARRANGE
   const auto& input = R"""(
    while(foo(1))
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
   
   std::vector< LexicalTokens::LexicalToken > scope_lexical_tokens{ lexical_tokens[8], lexical_tokens[9]  };
   const auto& scope = std::make_shared< ScopeSyntaxNode >( std::vector< ISyntaxNodeSP > {}, scope_lexical_tokens );
   std::vector< LexicalTokens::LexicalToken > while_lexical_tokens{ lexical_tokens[1] };
   const auto& while_statment = std::make_shared< WhileStatmentSyntaxNode >( function_call, scope, while_lexical_tokens );
   AbstractSyntaxTree expected_syntax_tree { while_statment };

   EXPECT_EQ( syntax_tree, expected_syntax_tree );
}
