#include "stack_machine.h"

#include "is_last_nodes.h"
#include "nonterminals/bin_expr_syntax_node.h"
#include "nonterminals/function_statment_syntax_node.h"
#include "nonterminals/if_statment_syntax_node.h"
#include "nonterminals/statment_syntax_node.h"
#include "nonterminals/function_call_syntax_node.h"
#include "nonterminals/return_statment_syntax_node.h"
#include "nonterminals/scope_statment_syntax_node.h"
#include "nonterminals/function_scope_statment_syntax_node.h"
#include "nonterminals/varible_assigment_statment_syntax_node.h"
#include "nonterminals/varible_syntax_node.h"
#include "nonterminals/while_statment_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "stack_machine.h"
#include "terminals/f_syntax_node.h"
#include "terminals/if_syntax_node.h"
#include "terminals/name_syntax_node.h"
#include "syntax_node_empty_visitor.h"
#include "lexical_tokens.h"
#include "abstract_syntax_tree.h"
#include "control_flow_graph.h"
#include "utils.h"
// #include "varible_store.h"
#include "VariableStore.h"
#include "i_syntax_node.h"
#include "copy_or_ref.h"

#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>

struct FunctionCallMeta
{
   ISyntaxNodeSP function;
   std::vector< ISyntaxNodeSP > arguments;
};

StackMachine::StackMachine( const ControlFlowGraph& cfg )
    : mCfg{ cfg }
{
}

Json StackMachine::exec()
{
   VariableStore1 varible_store;
   // varible_store.pushScope();

   std::deque< CopyOrRef<Json> > argument_stack;
   std::deque< FunctionCallSyntaxNodeSP > function_call_stack;
    
   std::function<Json(const ISyntaxNodeSP&)> get_value;
   get_value = [&argument_stack, &varible_store, &get_value](const ISyntaxNodeSP& value_syntax_node ) -> Json
    {   auto& s = argument_stack;
        (void) s;
        if( check_type<NameSyntaxNode>( value_syntax_node ) )
        {
            auto value = argument_stack.back();
            if( !argument_stack.empty() )
               argument_stack.pop_back();
        }
        else if( check_type<MemberExpressionSyntaxNode>( value_syntax_node ) )
        {
            auto key_or_index = get_value( value_syntax_node->operator[](1) );
            // if( !argument_stack.empty() )
               // argument_stack.pop_back();
            auto container = get_value( value_syntax_node->operator[](0) ); 
            // if( !argument_stack.empty() )
               // argument_stack.pop_back();
            // varible.get() = value;
             // auto& container = varible_store[ varible_name.get_string() ];
             if( container.is_array() )
             {
                if( key_or_index.is_int() )
                {
                   return container.get_array()[key_or_index.get_int()];
                }
                else
                {
                    throw std::runtime_error("Element of array can not be accesed not by index!");
                }
             }
             else if( container.is_object() )
             {
                if( key_or_index.is_string() )
                {
                   return container.get_object()[key_or_index.get_string()];
                }
                else
                {
                    throw std::runtime_error("Element of object can not be accesed not by key!");
                }
             }
        }
        else
        {
            auto value = argument_stack.back();
            if( !argument_stack.empty() )
               argument_stack.pop_back();
            return value.get();
        }
       return {};
    };

       SyntaxNodeEmptyVisitor::Handlers handlers;
       handlers.string_syntax_node = [ &argument_stack ]( const StringSyntaxNodeSP& node )
       {
           auto& s = argument_stack;
           (void) s;
           argument_stack.push_back( CopyOrRef<Json>{ node->value() } );
          // std::cout << "string = " << node->value() << std::endl;
       };
       handlers.int_syntax_node = [ &argument_stack ]( const IntSyntaxNodeSP& node )
       {
           auto& s = argument_stack;
           (void) s;
           argument_stack.push_back( CopyOrRef<Json>{ node->value() } );
          // std::cout << "f = " << node->value() << std::endl;
       };
       handlers.double_syntax_node = [ &argument_stack ]( const DoubleSyntaxNodeSP& node )
       {
           auto& s = argument_stack;
           (void) s;
           argument_stack.push_back( CopyOrRef<Json>{ node->value() } );
          // std::cout << "double = " << node->value() << std::endl;
       };
       handlers.name_syntax_node = [ &argument_stack, &varible_store ]( const NameSyntaxNodeSP& name )
       {
          argument_stack.push_back( CopyOrRef<Json>( name->value() ) );
          // std::cout << "name = " << name->value() << std::endl;
       };
       handlers.varible_syntax_node = [ &argument_stack, &varible_store ]( const VaribleSyntaxNodeSP& varible )
       {
          auto& s = argument_stack;
          (void) s;
          auto& varible_value_ref = varible_store[ varible->name() ];
          argument_stack.push_back( CopyOrRef<Json>( varible_value_ref ) );
          // std::cout << "varible: " << varible->name() << std::endl;
       };
       handlers.array_syntax_node = [ &argument_stack, &get_value ]( const ArraySyntaxNodeSP& node )
       {
          auto& s = argument_stack;
          (void) s;
          // std::cout << "f = " << node->value() << std::endl;
          std::vector<Json> arrays_elements;
          for( auto it=node->rbegin(); it!= node->rend(); ++it )
          {
             const auto& child = *it;
             auto value = argument_stack.back();
             if( !argument_stack.empty() )
                argument_stack.pop_back();
             // const auto& value = get_value( child );
             arrays_elements.insert( arrays_elements.begin(), value );
          }
          argument_stack.push_back( CopyOrRef<Json>{ arrays_elements } );
       };
       handlers.object_syntax_node = [ &argument_stack, &get_value ]( const ObjectSyntaxNodeSP& node )
       {
           auto& s = argument_stack;
           (void) s;
          // std::cout << "f = " << node->value() << std::endl;
          std::map<std::string, Json> object_elements;
          for( auto it=node->rbegin(); it!= node->rend(); ++it )
          {
             const auto& child = *it;
             auto value = argument_stack.back();
             if( !argument_stack.empty() )
                argument_stack.pop_back();
             // const auto& value = get_value( child->operator[](1) );
             auto key = argument_stack.back();
             if( !argument_stack.empty() )
                argument_stack.pop_back();
             // const auto& key = get_value( child->operator[](0) );
             object_elements.emplace( key.get().get_string(), value );
          }
          argument_stack.push_back( CopyOrRef<Json>{ object_elements } );
       };
       handlers.bin_expr_syntax_node = [ &argument_stack, &get_value ]( const BinExprSyntaxNodeSP& node )
       {
           auto& s = argument_stack;
           (void) s;
           // std::cout << "<<<STACK>>>>";
           // for (const auto& j : argument_stack )
           // {
           //     std::cout << j << ", ";
           // }
           // std::cout << "<<<STACK>>>>" << std::endl;
          // auto rhs = argument_stack.back().get();
          // if( !argument_stack.empty() )
          //    argument_stack.pop_back();
          // auto lhs = argument_stack.back().get();
          // if( !argument_stack.empty() )
          //    argument_stack.pop_back();
          // const auto& rhs = get_value( node->operator[](1) );
           auto rhs = argument_stack.back();
           if( !argument_stack.empty() )
              argument_stack.pop_back();
          // const auto& lhs = get_value( node->operator[](0) );
           auto lhs = argument_stack.back();
           if( !argument_stack.empty() )
              argument_stack.pop_back();
          if( node->type() == BinExprSyntaxNode::Type::Addition )
          {
            const auto& result = lhs + rhs;
            // std::cout << std::to_string( lhs ) << " + " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( result );
          }
          else if( node->type() == BinExprSyntaxNode::Type::Substruction )
          {
            const auto& result = lhs - rhs;
            // std::cout << std::to_string( lhs ) << " - " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( result );
          }
          else if( node->type() == BinExprSyntaxNode::Type::Multiply )
          {
            const auto& result = lhs * rhs;
            // std::cout << std::to_string( lhs ) << " * " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( result );
          }
          else if( node->type() == BinExprSyntaxNode::Type::Division )
          {
            const auto& result = lhs / rhs;
            // std::cout << std::to_string( lhs ) << " / " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( result );
          }
          else if( node->type() == BinExprSyntaxNode::Type::Equal )
          {
            const auto& result = lhs.get() == rhs.get();
            // std::cout << lhs.get() << " == " << rhs.get() << " = " << result << std::endl;
            argument_stack.push_back( CopyOrRef<Json>{ result } );
          }
          else if( node->type() == BinExprSyntaxNode::Type::Less )
          {
            const auto& result = lhs < rhs;
            // std::cout << std::to_string( lhs ) << " < " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( CopyOrRef<Json>{ result } );
          }
          else if( node->type() == BinExprSyntaxNode::Type::LessEqual )
          {
            const auto& result = lhs <= rhs;
            // std::cout << std::to_string( lhs ) << " <= " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( CopyOrRef<Json>{ result } );
          }
          else if( node->type() == BinExprSyntaxNode::Type::More )
          {
            const auto& result = lhs > rhs;
            // std::cout << std::to_string( lhs ) << " > " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( CopyOrRef<Json>{ result } );
          }
          else if( node->type() == BinExprSyntaxNode::Type::MoreEqual )
          {
            const auto& result = lhs >= rhs;
            // std::cout << std::to_string( lhs ) << " >= " << std::to_string( rhs ) << " = " << std::to_string( result ) << std::endl;
            argument_stack.push_back( CopyOrRef<Json>{ result } );
          }
       };
       handlers.scope_statment_syntax_node = [ &varible_store ]( const ScopeSyntaxNodeSP& scope )
       {
          // delete scope in a VaribleStore
          varible_store.popScope();
          // argument_stack.clear();
          // std::cout << "Scope end" << std::endl;
       };
       handlers.function_scope_statment_syntax_node = [ &varible_store ]( const FunctionScopeSyntaxNodeSP& scope )
       {
          // delete scope in a VaribleStore
          varible_store.popScope();
          varible_store.popScope();
          // argument_stack.clear();
          // std::cout << "Scope end" << std::endl;
       };
       handlers.return_statment_syntax_node = [ &function_call_stack, &argument_stack, &varible_store, &get_value ]( const ReturnStatmentSyntaxNodeSP& return_statment )
       {
           auto& s = argument_stack;
           (void) s;
           // const auto& value = get_value( return_statment->operator[](0) );
           auto value = argument_stack.back();
           if( !argument_stack.empty() )
              argument_stack.pop_back();
           // const auto& v = value.get();
           // std::cout << "before return " << value.get() << std::endl;
           const auto& c = value.copy();
           // std::cout << "return " << c.get() << std::endl;
           argument_stack.push_back( c );
       };
       handlers.print_statment_syntax_node = [ &argument_stack, &varible_store, &get_value ]( const PrintStatmentSyntaxNodeSP& print_statment )
       {
           auto& s = argument_stack;
           (void) s;
           auto& d = varible_store;
           (void) d;
           auto value = argument_stack.back();
           if( !argument_stack.empty() )
              argument_stack.pop_back();
           // return varible_name.get().get_int();
           // const auto& value = get_value( print_statment->operator[](0) );
           // std::cout << "print: " << value.get() << std::endl;
           if( value.get().is_string() )
               std::printf("%s\n", value.get().get_string().c_str());
           else if( value.get().is_int() )
               std::printf("%d\n", value.get().get_int());
           else if( value.get().is_double() )
               std::printf("%f\n", value.get().get_double());
           else if( value.get().is_bool() )
               std::printf("%i\n", value.get().get_bool());
           else if( value.get().is_function() )
               std::printf("%s\n", value.get().get_function().get_text().c_str());
           else 
               std::cout << value.get() << std::endl;
       };
       handlers.member_expression_syntax_node = [ &argument_stack, &varible_store, &get_value ]( const MemberExpressionSyntaxNodeSP& member_expression )
       {
           auto& s = argument_stack;
           (void) s;
           auto key_or_index = argument_stack.back().get();
           if( !argument_stack.empty() )
              argument_stack.pop_back();
           auto& container = argument_stack.back().get();
           if( !argument_stack.empty() )
              argument_stack.pop_back();
           if( container.is_array() )
           {
              if( key_or_index.is_int() )
              {
                 std::vector<Json>& container_as_array = container.get_array();
                 const auto& index = key_or_index.get_int();
                 if( index < 0 )
                 {
                     argument_stack.push_back( CopyOrRef<Json>({}) );
                     return;
                 }
                 if( container_as_array.size() <= index )
                 {
                     container_as_array.resize( index+1 );
                 }
                 auto& value_by_ref = container_as_array.at( index );
                 argument_stack.push_back( CopyOrRef<Json>{ value_by_ref } );
              }
              else
              {
                  throw std::runtime_error("Element of array can not be accesed not by index!");
              }
           }
           else if( container.is_object() )
           {
              if( key_or_index.is_string() )
              {
                 auto& value_by_ref = container.get_object()[key_or_index.get_string()];
                 argument_stack.push_back( CopyOrRef<Json>{ value_by_ref } );
              }
              else
              {
                  throw std::runtime_error("Element of object can not be accesed not by key!");
              }
           }
           // const auto& value = get_value( member_expression );
           // argument_stack.push_back( CopyOrRef<Json>{ value_by_ref } );
           // std::cout << "member_expression value: " << value << std::endl;
       };
       handlers.varible_assigment_statment_syntax_node = [ &varible_store, &argument_stack, &get_value ]( const VaribleAssigmentStatmentSyntaxNodeSP& varible_assigment )
       {
          // const auto& source = varible_assigment->source();
          auto& s = argument_stack;
          (void) s;
          // std::cout << "BEFORE ASSIGMENT" << std::endl;
          // varible_store.print();
          
          if( check_type<NameSyntaxNode>( varible_assigment->operator[]( 0 ) ) )
          {
              if( varible_assigment->Children().size() == 2 )
              {
                  // const auto& varible_value = get_value( varible_assigment->operator[](1) );
                  auto varible_value = argument_stack.back().get();
                  if( !argument_stack.empty() )
                     argument_stack.pop_back();
                  
                  auto varible_name = argument_stack.back().get();
                  if( !argument_stack.empty() )
                     argument_stack.pop_back();
                  // varible.get() = value;
                  std::string context;
                  if( varible_assigment->context() == VaribleAssigmentStatmentSyntaxNode::Context::GLOBAL )
                  {
                     // varible_store[ varible_name.get_string() ] = varible_value;
                     varible_store.write(varible_name.get_string(), varible_value, EnvScope::VaribleType::Global);
                     context = "Global";
                  }
                  else if( varible_assigment->context() == VaribleAssigmentStatmentSyntaxNode::Context::LOCAL )
                  {
                     varible_store.writeValueToLocalVarible( varible_name.get_string(), varible_value );
                     context = "Local";
                  }
                  // std::cout << "Write " << target_name << " .Value is " << value << ". Context: " << context << std::endl;
              }
              else 
              {
                  // const auto& varible_value = get_value( varible_assigment->operator[](1) );
                  auto varible_name = argument_stack.back().get();
                  if( !argument_stack.empty() )
                     argument_stack.pop_back();
                  
                  auto varible_value = argument_stack.back().get();
                  if( !argument_stack.empty() )
                     argument_stack.pop_back();
                  
                  // varible.get() = value;
                  std::string context;
                  if( varible_assigment->context() == VaribleAssigmentStatmentSyntaxNode::Context::GLOBAL )
                  {
                     // varible_store[ varible_name.get_string() ] = varible_value;
                     varible_store.write(varible_name.get_string(), varible_value, EnvScope::VaribleType::Global);
                     context = "Global";
                  }
                  else if( varible_assigment->context() == VaribleAssigmentStatmentSyntaxNode::Context::LOCAL )
                  {
                     varible_store.writeValueToLocalVarible( varible_name.get_string(), varible_value );
                     context = "Local";
                  }
                  // std::cout << "Write " << target_name << " .Value is " << value << ". Context: " << context << std::endl;
              }            
            
          }
          else if( check_type<MemberExpressionSyntaxNode>( varible_assigment->operator[]( 0 ) ) )
          {
              // varible_store.print();
              auto varible_value = argument_stack.back();
              if( !argument_stack.empty() )
                 argument_stack.pop_back();
              
              auto target = argument_stack.back();
              if( !argument_stack.empty() )
                 argument_stack.pop_back();
              target.get() = varible_value;
              // varible_store.print();
              
              // const auto& member_value = get_value( varible_assigment->operator[](1) );
              // auto key_or_index = argument_stack.back();
              // if( !argument_stack.empty() )
              //    argument_stack.pop_back();
              // auto varible_name = argument_stack.back();
              // if( !argument_stack.empty() )
              //    argument_stack.pop_back();
              // if( varible_assigment->context() == VaribleAssigmentStatmentSyntaxNode::Context::GLOBAL )
              // {
              //    auto& container = varible_store[ varible_name.get().get_string() ];
              //    if( container.is_array() )
              //    {
              //       if( key_or_index.get().is_int() )
              //       {
              //          container.get_array()[key_or_index.get().get_int()] = member_value;
              //       }
              //       else
              //       {
              //           throw std::runtime_error("Element of array can not be accesed not by index!");
              //       }
              //    }
              //    else if( container.is_object() )
              //    {
              //       if( key_or_index.get().is_string() )
              //       {
              //          container.get_object()[key_or_index.get().get_string()] = member_value;
              //       }
              //       else
              //       {
              //           throw std::runtime_error("Element of object can not be accesed not by key!");
              //       }
              //    }
              // }
          }
          // std::cout << "AFTER ASSIGMENT" << std::endl;
          // varible_store.print();
       };
       handlers.function_call_syntax_node = [ &function_call_stack, &varible_store ]( const FunctionCallSyntaxNodeSP& function_call )
       { 
           function_call_stack.pop_back();
           // varible_store.popScope();
       };
       const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
   std::vector< ISyntaxNodeSP > stack;
   const auto& post_func = [ &visitor, &varible_store, &argument_stack, &function_call_stack, &stack, &get_value ]( const ISyntaxNodeSP& node )
    {
       node->accept( visitor );

     
       stack.pop_back();
   };
   const auto& pre_func = [ &varible_store, &function_call_stack, &argument_stack, &stack, &get_value, &post_func ]( const ISyntaxNodeSP& node, StackDFS< ISyntaxNodeSP >& stack_dfs )
  {
      stack.emplace_back( node );
     auto children = node->Children();
     SyntaxNodeEmptyVisitor::Handlers handlers;
     handlers.scope_statment_syntax_node =
        [ &varible_store ]( const ScopeSyntaxNodeSP& scope )

     {
        // create scope in a VaribleStore
        varible_store.pushScope();
        // std::cout << "Scope begin" << std::endl;
     },
     handlers.function_scope_statment_syntax_node =
        [ &varible_store, &function_call_stack ]( const FunctionScopeSyntaxNodeSP& function_scope )

     {
          auto& s = varible_store;
          (void) s;
          auto function_ref = varible_store[ function_call_stack.back()->name() ];
          if( function_ref.is_function() )
          {
              const auto& function_syntax_node = std::dynamic_pointer_cast<FunctionStatmentSyntaxNode>( function_ref.get_function().get_ast() );
              varible_store.pushScope( function_ref.get_function().getScope() );
          }
          varible_store.pushScope();
     },
     handlers.if_statment_syntax_node = [ &children, &argument_stack, &get_value, &varible_store, &stack_dfs ]( const IfStatmentSyntaxNodeSP& if_statment )
     {
        auto& ss = stack_dfs;
        (void) ss;
        // CopyOrRef<Json> condition_result;
        if( !argument_stack.empty() )
        {
            // varible_store.print();
            auto& s = argument_stack;
            (void) s;
            const auto& condition = get_value( if_statment->conditional_expression() );
            if( !condition.is_null() )
            {
               if( ( condition.is_double()   && condition.get_double() > 0.0 ) || 
                   ( condition.is_int()      && condition.get_int() > 0 )      || 
                   ( condition.is_bool()     && condition.get_bool() == true ) ||
                   ( condition.is_function() ) ) 
               {
                   const auto& true_scope = if_statment->true_scope();
                   children = std::list< ISyntaxNodeSP >{ true_scope };
                   return;
               }    
               else
               {
                   const auto& false_scope = if_statment->false_scope();
                   if( !false_scope )
                       children = {};
                   else
                       children = std::list< ISyntaxNodeSP >{ false_scope };
                   return;
               }    
               children = {};
            }
            // argument_stack.pop_back();
        }
        else
        {
           children = {};
        }
     };
     handlers.goto_syntax_node = [ &children, &stack_dfs, &post_func ]( const GotoSyntaxNodeSP& goto_syntax_node )
     {
        const auto& target = goto_syntax_node->target();
        stack_dfs.cancelDescentFromSubtree(target, post_func);
        children = target->Children();
     };
     handlers.function_statment_syntax_node = [ &children, &varible_store ]( const FunctionStatmentSyntaxNodeSP& function )
     {
        auto& d = varible_store;
        (void) d;
        auto tokens = function->lexical_tokens();
        
        const auto& whole_childrens = DfsRange< ISyntaxNodeSP >{ function->Children() };
        for( const auto& child : whole_childrens )
        {
            const auto& child_tokens = child.get()->lexical_tokens();
            tokens.insert(tokens.end(), child_tokens.begin(), child_tokens.end());
        }

        std::sort( tokens.begin(), tokens.end(), []( const LexicalTokens::LexicalToken& a, const LexicalTokens::LexicalToken& b )
        {
            if( a.line != b.line )
                return a.line < b.line;
            return a.col < b.col;
        });

        std::string function_text;
        size_t current_line = 0;
        size_t current_col = 0;
        for( const auto& token : tokens )
        {
            if( current_line == 0 && current_col == 0 )
            {
                current_line = token.line;
                current_col = token.col;
            }

            if( token.line > current_line )
            {
                size_t line_diff = token.line - current_line;
                for( size_t i = 0; i < line_diff; ++i )
                    function_text += '\n';
                current_line = token.line;
                current_col = 0;
            }

            if( token.col > current_col )
            {
                size_t col_diff = token.col - current_col;
                for( size_t i = 0; i < col_diff; ++i )
                    function_text += ' ';
            }

            function_text += token.text;
            current_col = token.col + token.length;
        }

        auto c = varible_store.getCurrentScope();
          // std::cout << "current env: " << reinterpret_cast<uintptr_t>(c.get()) << std::endl;
        const json::Function f{c, function_text, function};
        auto& ss = varible_store;
        (void) ss;
          // std::cout << "FUNCTION_DECL" << std::endl;
        varible_store.write( function->name(), f, EnvScope::VaribleType::Local);
        // varible_store.print();
        
        children = {};
     };
     handlers.function_call_syntax_node = [ &function_call_stack, &argument_stack, &varible_store, &children ]( const FunctionCallSyntaxNodeSP& function_call )
     { 
          // std::cout << "function_call " << function_call->name() << std::endl;
          auto& s = argument_stack;
          (void) s;
          auto& a = children;
          (void) a;
          auto& b = varible_store;
          (void) b;
          auto function_ref = varible_store[ function_call->name() ];
          if( function_ref.is_function() )
          {
              const auto& function_syntax_node = std::dynamic_pointer_cast<FunctionStatmentSyntaxNode>( function_ref.get_function().get_ast() );
              // varible_store.pushScope( function_ref.get_function().getScope() );
              children.push_back( function_syntax_node->scope() ); 
          }
          // varible_store.print();
         
          function_call_stack.emplace_back( function_call );
     };
     // handlers.varible_assigment_statment_syntax_node = [ &children ]( const VaribleAssigmentStatmentSyntaxNodeSP& varible_assigment )
     // {
     //    const auto& source = varible_assigment->source();
     //    children = {source};
     // };
     handlers.return_statment_syntax_node = [ &stack_dfs, &function_call_stack, &argument_stack, &varible_store ]( const ReturnStatmentSyntaxNodeSP& /* return_statment */ )
     {
          // std::cout << "RETURN" << std::endl;
          // varible_store.print();
         if( !stack_dfs.empty() )
         {
           auto& d = function_call_stack ;
           (void) d;
            stack_dfs.unsheduleDescentToSubtree( !function_call_stack.empty() ? function_call_stack.back() : nullptr );
         }
         // if( !argument_stack.empty() )
         // {
         //    auto result = argument_stack.back();
         //    // argument_stack.pop_back();
         //
         //    std::cout << "return: " << std::to_string( result ) << std::endl;
         // }
     };
     const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
     node->accept( visitor );
     
     stack_dfs.push( children );
  };
   
   iterative_managed_dfs(
      mCfg.root(),
      pre_func,
      post_func );
    
    // varible_store.print();
    // std::cout << "<<<< stack >>>>" << std::endl;
    // for( const auto& elem : argument_stack )
    // {
    //   std::cout << "-> " << elem.get() << std::endl;
    // }
    // std::cout << "<<<< stack >>>>" << std::endl;

    Json result;
    if( !argument_stack.empty() )
    {
        result = argument_stack.back().get();
        argument_stack.pop_back();
    }
    return result;
}
