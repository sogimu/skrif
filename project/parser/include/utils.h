#pragma once

#include "i_grammar.h"
#include "i_syntax_node.h"
#include "nonterminals/member_expression_syntax_node.h"
#include "syntax_node_empty_visitor.h"
#include "syntax_node_progress_visitor.h"
#include "utils/dfs_pre_order_range.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <stdexcept>
#include <vector>
#include <list>
#include <utility>
#include <cmath>
#include <unordered_set>
#include <sstream>

template< typename Node, typename PreFunc, typename PostFunc >
void iterative_dfs( const Node& start, PreFunc pre_func, PostFunc post_func )
{
   std::stack< std::pair< Node, bool > > stack;
   stack.emplace( start, true );

   while( !stack.empty() )
   {
      auto [ node, is_pre_order ] = stack.top();
      stack.pop();

      if( is_pre_order )
      {
         stack.emplace( node, false );
         bool is_found = pre_func( node );

         if( !is_found )
         {
            const auto& childern = node->Children();
            for( auto it = childern.rbegin(); it != childern.rend(); ++it )
            {
               const auto& child = *it;
               stack.emplace( child, true );
            }
         }
      }
      else
      {
         post_func( node );
      }
   }
}

template< typename Node, typename PreFunc >
void iterative_dfs( const Node& start, PreFunc pre_func )
{
   iterative_dfs( start, pre_func, []( const auto& ) {} );
}

template< typename Node >
class StackDFS
{
public:
   StackDFS( std::vector< std::pair< Node, bool > >& stack )
      : mStack{ stack }
   {
   }

   void push( const ISyntaxNodeSP& node, bool is_pre_order )
   {
      mStack.emplace_back( node, is_pre_order );
   }

   void push( const std::list< ISyntaxNodeSP >& nodes )
   {
      for( auto it = nodes.rbegin(); it != nodes.rend(); ++it )
      {
         const auto& child = *it;
         push( child, true );
      }
   }

   void pushChildrenOf( const ISyntaxNodeSP& node )
   {
      const auto& childern_value = node->Children();
      push( childern_value, true );
   }
   
   void cancelDescentFromSubtree( const ISyntaxNodeSP& subtree_root, const std::function<void(const ISyntaxNodeSP&)>& post_func )
   {
      for( auto it = mStack.rbegin(); it != mStack.rend(); )
      {
         // auto& [ node, is_pre_order ] = stack.top();
         auto& [ node, is_pre_order ] = *it;
         if( node == subtree_root )
            break;
         if( is_pre_order == false )
         {
            post_func( node ); 
            it = decltype( it )( mStack.erase( std::next( it ).base() ) );
         }
         else
         {
            ++it;
         }
      }
   }

   void unsheduleDescentToSubtree( const ISyntaxNodeSP& subtree_root )
   {
      for( auto it = mStack.rbegin(); it != mStack.rend(); )
      {
         // auto& [ node, is_pre_order ] = stack.top();
         auto& [ node, is_pre_order ] = *it;
         if( node == subtree_root )
            break;
         if( is_pre_order )
         {
            it = decltype( it )( mStack.erase( std::next( it ).base() ) );
         }
         else
         {
            ++it;
         }
      }
   }

   bool empty() const
   {
      return mStack.empty();
   }

   auto top() const
   {
      return mStack.back();
   }
   void pop()
   {
      mStack.pop_back();
   }

   auto begin()
   {
      return mStack.begin();
   }
   
   auto end()
   {
      return mStack.end();
   }

private:
   std::vector< std::pair< Node, bool > >& mStack;
};

template< typename Node, typename PreFunc, typename PostFunc >
void iterative_managed_dfs( const Node& start, PreFunc pre_func, PostFunc post_func )
{
   std::vector< std::pair< Node, bool > > stack;

   StackDFS< Node > stack_dfs{ stack };
   stack_dfs.push( start, true );
   // stack.emplace( start, true );

   // while( !stack.empty() )
   while( !stack_dfs.empty() )
   {
      // auto& [ node, is_pre_order ] = stack.top();
      auto [ node, is_pre_order ] = stack_dfs.top();
      stack_dfs.pop();

      if( is_pre_order )
      {
         stack_dfs.push( node, false );
         /* const auto& children_opt =  */ pre_func( node, stack_dfs );
         // stack.emplace( node, false );
         // is_pre_order = false;

         // if( !children_opt )
         //    continue;
         // const auto& childern_value = children_opt.value();
         // for( auto it = childern_value.rbegin(); it != childern_value.rend();
         // ++it )
         // {
         //    const auto& child = *it;
         //    stack.emplace( child, true );
         // }
      }
      else
      {
         post_func( node, stack_dfs );
         // stack.pop();
         // stack_dfs.pop();
      }
   }
}

template< typename Node, typename PreFunc >
void iterative_managed_dfs( const Node& start, PreFunc pre_func )
{
   iterative_managed_dfs( start, pre_func, []( const auto& ) {} );
}

template< typename Node, typename PreFunc, typename PostFunc, typename GetChildrenReverseIterators >
void iterative_dfs2( const Node& start, const PreFunc& pre_func, const PostFunc& post_func, const GetChildrenReverseIterators& get_children_reverse_iterators )
{
   std::stack< std::pair< Node, bool > > stack;
   stack.emplace( start, true );

   while( !stack.empty() )
   {
      auto [ node, is_pre_order ] = stack.top();
      stack.pop();

      if( is_pre_order )
      {
         stack.emplace( node, false );
         bool is_found = pre_func( node );

         if( !is_found )
         {
            const auto& [ rbegin, rend ] = get_children_reverse_iterators( node );
            for( auto it = rbegin; it != rend; ++it )
            {
               const auto& c = *it;
               stack.emplace( c, true );
            }
         }
      }
      else
      {
         post_func( node );
      }
   }
}

template< typename Node, typename PreFunc, typename GetChildrenReverseIterators >
void iterative_dfs2( const Node& start, const PreFunc& pre_func, const GetChildrenReverseIterators& get_children_reverse_iterators )
{
   iterative_dfs2( start, pre_func, []( const auto& ) {}, get_children_reverse_iterators );
}
// helper type for the visitor #4
template< class... Ts >
struct overloaded : Ts...
{
   using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template< class... Ts >
overloaded( Ts... ) -> overloaded< Ts... >;

using Stack = std::deque< ISyntaxNodeSP >;
static Stack last_n_syntax_nodes( const Stack& stack, size_t n )
{
   if( stack.size() < n )
      return {};

   auto it = stack.begin();
   std::advance( it, stack.size() - n );

   return Stack{ it, stack.end() };
}

[[maybe_unused]] static void iterate_over_last_n_nodes( const Stack& stack, size_t n, const SyntaxNodeEmptyVisitor::Handlers& handlers )
{
   const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
   const auto& nodes = last_n_syntax_nodes( stack, n );
   for( const auto& node : nodes )
   {
      if( !node )
         throw std::runtime_error( "Node is not exist" );
      node->accept( visitor );
   }
}
template<typename State>
struct Result
{
  State state;
  double readiness;
};
template< typename State >
[[maybe_unused]] static Result< State > iterate_over_last_n_nodesv2( const Stack& stack, size_t n, const typename SyntaxNodeProgressVisitor< State >::Handlers& handlers )
{
   const auto& visitor = std::make_shared< SyntaxNodeProgressVisitor< State > >( handlers );
   const auto& nodes = last_n_syntax_nodes( stack, n );
   for( const auto& node : nodes )
   {
      if( !node )
         throw std::runtime_error( "Node is not exist" );
      node->accept( visitor );
   }
   return Result<State>{ visitor->state(), visitor->readiness() };
}

[[maybe_unused]] static IGrammar::PlanOrProgress find_grammar_matching_progress( const Stack& stack, const size_t needed_nodes_size, const std::function<IGrammar::PlanOrProgress(size_t n)>& find_progress_over_last_n_nodes )
{
    const auto& stack_size = stack.size();

    size_t start_index = 0;

    if( stack_size >= needed_nodes_size )
    {
        start_index = stack_size-needed_nodes_size;
    }

    IGrammar::Progress max_progress{ .readiness = 0.0 };
    std::optional<IGrammar::Plan> plan_opt;

    for( size_t n=needed_nodes_size; n>0; --n )
    { 
        const auto& plan_or_progress = find_progress_over_last_n_nodes( n );
        std::visit( [ &max_progress, &plan_opt ]( auto&& arg )
        {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, IGrammar::Progress>)
          {
            if( arg.readiness > max_progress.readiness )
              max_progress = arg;
          } 
          else if constexpr (std::is_same_v<T, IGrammar::Plan>) 
          {
              plan_opt = arg;
          }          
        }, plan_or_progress );
        if( plan_opt )
          return plan_opt.value();
    }

    return max_progress;
}

template< typename Range >
class zip_container
{
   Range c1;
   Range c2;

public:
   zip_container( const Range& c1, const Range& c2 )
      : c1( c1 )
      , c2( c2 )
   {
   }

   class Iterator
   {
   public:
      using iterator_category = std::input_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = std::pair< std::optional< typename Range::Iterator::value_type >, std::optional< typename Range::Iterator::value_type > >;
      using reference = value_type&;
      using pointer = value_type*;

      Iterator( const auto& c1, const auto& c1end, const auto& c2, const auto& c2end )
         : mC1{ c1 }
         , mC1End{ c1end }
         , mC2( c2 )
         , mC2End{ c2end }
      {
      }

      value_type operator*() const noexcept
      {
         std::optional< typename Range::Iterator::value_type > a;
         if( mC1 != mC1End )
            a = *mC1;
         std::optional< typename Range::Iterator::value_type > b;
         if( mC2 != mC2End )
            b = *mC2;
         return { a, b };
      }

      Iterator operator++()
      {
         ++mC1;
         ++mC2;
         return *this;
      }

      bool operator!=( const Iterator& iterator ) const noexcept
      {
         return mC1 != iterator.mC1 || mC2 != iterator.mC2;
         return false;
      }

      typename Range::Iterator mC1;
      typename Range::Iterator mC1End;
      typename Range::Iterator mC2;
      typename Range::Iterator mC2End;
   };

   auto begin() const
   {
      return Iterator( std::begin( c1 ), std::end( c1 ), std::begin( c2 ), std::end( c2 ) );
   }

   auto end() const
   {
      return Iterator( std::end( c1 ), std::end( c1 ), std::end( c2 ), std::end( c2 ) );
   }
};

template< typename C1 >
zip_container< C1 > zip( C1& c1, C1& c2 )
{
   return zip_container< C1 >( c1, c2 );
}

[[maybe_unused]] static void match( const ISyntaxNodeSP& node, const SyntaxNodeEmptyVisitor::Handlers& handlers )
{
   const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
   node->accept( visitor );
}

template< typename SourceNode, typename TargetNode, typename CreateRoot, typename CreateNode, typename GetChildrenReverseIterators >
TargetNode create_tree_from( const SourceNode& source_root, const CreateRoot& create_root, const CreateNode& create_node,
                             const GetChildrenReverseIterators& get_children_reverse_iterators )
{
   std::vector< std::pair< std::reference_wrapper< const SourceNode >, std::optional< std::reference_wrapper< TargetNode > > > > stack;
   stack.emplace_back( std::make_pair( std::cref( source_root ), std::optional< std::reference_wrapper< TargetNode > >{} ) );
   std::vector< std::reference_wrapper< const TargetNode > > source_stack;
   source_stack.emplace_back( std::cref( source_root ) );
   std::vector< std::reference_wrapper< TargetNode > > target_stack;
   auto target_root = create_root( source_root );
   target_stack.emplace_back( std::ref( target_root ) );
   stack.emplace_back( std::make_pair( std::cref( source_root ), std::optional< std::reference_wrapper< TargetNode > >{ std::ref( target_root ) } ) );
   const auto& [ rbegin, rend ] = get_children_reverse_iterators( source_root );
   // size_t root_children_size = std::distance( rbegin, rend );
   std::vector< std::reference_wrapper< const SourceNode > > source_root_children_forward_order( rbegin, rend );
   std::reverse( source_root_children_forward_order.begin(), source_root_children_forward_order.end() );
   for( const auto& child_of_root : source_root_children_forward_order )
   {
      iterative_dfs2(
         child_of_root,
         [ &create_node, &stack, &target_stack, &source_stack ]( const SourceNode& source_node ) -> bool
         {
            source_stack.emplace_back( std::cref( source_node ) );
            std::optional< std::reference_wrapper< TargetNode > > new_target_node_opt = create_node( source_stack, target_stack );
            stack.emplace_back( std::make_pair( std::cref( source_node ), new_target_node_opt ) );
            if( new_target_node_opt )
            {
               target_stack.emplace_back( new_target_node_opt.value() );
            }
            return false;
         },
         [ &stack, &source_stack, &target_stack ]( [[maybe_unused]] const SourceNode& source_node )
         {
            auto& [ source, target ] = stack.back();
            if( target )
            {
               target_stack.pop_back();
            }
            source_stack.pop_back();
            stack.pop_back();
         },
         get_children_reverse_iterators );
   }
   return target_root;
}

// Шаблон для извлечения типа объекта из Ptr

template <typename Ptr, typename = void>
struct pointee {
    // По умолчанию считаем, что Ptr — это уже тип объекта
    using type = Ptr;
};

template <typename Ptr>
struct pointee<Ptr, std::void_t<typename Ptr::element_type>> {
    // Если есть element_type (умный указатель), берем его
    using type = typename Ptr::element_type;
};

template <typename T>
struct pointee<T*, void> {
    // Если Ptr — обычный указатель T*
    using type = T;
};

template< typename SourceNode, typename TargetNode, typename CreateRoot, typename CreateNodePre, typename CreateNodePost, typename GetChildrenReverseIterators >
TargetNode create_tree_from1( const SourceNode& source_root, const CreateRoot& create_root, const CreateNodePre& create_node_pre_func,
                              const CreateNodePost& create_node_on_post_func, const GetChildrenReverseIterators& get_children_reverse_iterators )
{
   using CleanTargetNode = typename pointee<TargetNode>::type;
   using TargetIterator = decltype(std::declval<CleanTargetNode>().begin()); 
   
   std::vector< std::pair< const SourceNode, bool > > stack;
   stack.emplace_back( std::make_pair( source_root, false ) );
   std::vector< TargetNode > source_stack;
   source_stack.emplace_back( source_root );
   std::vector< std::pair<TargetNode, TargetIterator> > target_stack;
   auto target_root = create_root( source_root );
   target_stack.emplace_back( target_root, target_root->end() );
   stack.emplace_back( std::make_pair( source_root, target_root ) );
   const auto& [ rbegin, rend ] = get_children_reverse_iterators( source_root );
   // size_t root_children_size = std::distance( rbegin, rend );
   std::vector< SourceNode > source_root_children_forward_order( rbegin, rend );
   std::reverse( source_root_children_forward_order.begin(), source_root_children_forward_order.end() );
   for( const auto& child_of_root : source_root_children_forward_order )
   {
      iterative_dfs2(
         child_of_root,
         [ &create_node_pre_func, &stack, &target_stack, &source_stack ]( const SourceNode& source_node ) -> bool
         {
            source_stack.emplace_back( std::cref( source_node ) );
            // std::optional< std::reference_wrapper< TargetNode > > new_target_node_opt = create_node_pre_func( source_stack, target_stack );
            const auto& [ new_target_parent, call_post_func, it]  = create_node_pre_func( source_stack, target_stack );
            stack.emplace_back( std::make_pair( source_node, call_post_func ) );
            target_stack.emplace_back( new_target_parent, it );
            return false;
         },
         [ &create_node_on_post_func, &stack, &source_stack, &target_stack ]( [[maybe_unused]] const SourceNode& source_node )
         {
            const auto& [ source, call_post_func ] = stack.back();
            if( call_post_func )
            {
               create_node_on_post_func( source_stack, target_stack );
            }
            target_stack.pop_back();
            source_stack.pop_back();
            stack.pop_back();
         },
         get_children_reverse_iterators );
   }
   return target_root;
}

template< typename Node, typename PreFunc, typename PostFunc, typename GetChildrenReverseIterators >
void iterative_dfs3( const Node& root, const PreFunc& pre_func, const PostFunc& post_func, const GetChildrenReverseIterators& get_children_reverse_iterators )
{
   std::vector< Node > stack;
    iterative_dfs2(
       root,
       [ &stack, pre_func]( const Node& node ) -> bool
       {
          stack.emplace_back( std::cref( node ) );
          return pre_func(stack);
       },
       [ &stack, post_func ]( [[maybe_unused]] const Node& node )
       {
          post_func(stack);
          stack.pop_back();
       },
       get_children_reverse_iterators );
}

namespace
{
template< typename T >
class SyntaxNodeCheckTypeVisitor : public ISyntaxNodeVisitor
{
public:
   ~SyntaxNodeCheckTypeVisitor() = default;

   void visit( const BolSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, BolSyntaxNode > )
         mResult = true;
   }

   void visit( const EolSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, EolSyntaxNode > )
         mResult = true;
   }

   void visit( const PlusSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, PlusSyntaxNode > )
         mResult = true;
   }

   void visit( const MinusSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, MinusSyntaxNode > )
         mResult = true;
   }

   void visit( const AsteriskSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, AsteriskSyntaxNode > )
         mResult = true;
   }
   
   void visit( const VarSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, VarSyntaxNode > )
         mResult = true;
   }
   
   void visit( const SlashSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, SlashSyntaxNode > )
         mResult = true;
   }

   void visit( const DoubleSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, DoubleSyntaxNode > )
         mResult = true;
   }
   
   void visit( const IntSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, IntSyntaxNode > )
         mResult = true;
   }

   void visit( const FSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, FSyntaxNode > )
         mResult = true;
   }

   void visit( const UnExprSyntaxNodeSP& ) override
   {
      if constexpr( std::is_same_v< T, UnExprSyntaxNode > )
         mResult = true;
   }
  
   void visit( const BinExprSyntaxNodeSP& ) override
   {
      if constexpr( std::is_same_v< T, BinExprSyntaxNode > )
         mResult = true;
   }
   
   void visit( const MemberExpressionSyntaxNodeSP& ) override
   {
      if constexpr( std::is_same_v< T, MemberExpressionSyntaxNode > )
         mResult = true;
   }

   void visit( const SemicolonSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, SemicolonSyntaxNode > )
         mResult = true;
   }

   void visit( const ColonSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ColonSyntaxNode > )
         mResult = true;
   }

   void visit( const DotSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, DotSyntaxNode > )
         mResult = true;
   }

   void visit( const StatmentSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, StatmentSyntaxNode > )
         mResult = true;
   }

   void visit( const ScopeSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ScopeSyntaxNode > )
         mResult = true;
   }

   void visit( const FunctionScopeSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, FunctionScopeSyntaxNode > )
         mResult = true;
   }

   void visit( const ArraySyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ArraySyntaxNode > )
         mResult = true;
   }

   void visit( const ObjectSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ObjectSyntaxNode > )
         mResult = true;
   }

   void visit( const ObjectPairSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ObjectPairSyntaxNode > )
         mResult = true;
   }

   void visit( const OpenCurlyBracketSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, OpenCurlyBracketSyntaxNode > )
         mResult = true;
   }

   void visit( const CloseCurlyBracketSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, CloseCurlyBracketSyntaxNode > )
         mResult = true;
   }

   void visit( const OpenCircleBracketSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, OpenCircleBracketSyntaxNode > )
         mResult = true;
   }

   void visit( const CloseCircleBracketSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, CloseCircleBracketSyntaxNode > )
         mResult = true;
   }

   void visit( const OpenSquareBracketSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, OpenSquareBracketSyntaxNode > )
         mResult = true;
   }

   void visit( const CloseSquareBracketSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, CloseSquareBracketSyntaxNode > )
         mResult = true;
   }

   void visit( const VaribleSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, VaribleSyntaxNode > )
         mResult = true;
   }

   void visit( const PrintStatmentSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, PrintStatmentSyntaxNode > )
         mResult = true;
   }

   void visit( const VaribleAssigmentStatmentSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, VaribleAssigmentStatmentSyntaxNode > )
         mResult = true;
   }

   void visit( const NameSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, NameSyntaxNode > )
         mResult = true;
   }

   void visit( const StringSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, StringSyntaxNode > )
         mResult = true;
   }

   void visit( const CommaSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, CommaSyntaxNode > )
         mResult = true;
   }

   void visit( const PrintSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, PrintSyntaxNode > )
         mResult = true;
   }

   void visit( const EqualSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, EqualSyntaxNode > )
         mResult = true;
   }

   void visit( const NotSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, NotSyntaxNode > )
         mResult = true;
   }

   void visit( const LessSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, LessSyntaxNode > )
         mResult = true;
   }

   void visit( const MoreSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, MoreSyntaxNode > )
         mResult = true;
   }

   void visit( const IfSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, IfSyntaxNode > )
         mResult = true;
   }

   void visit( const ElseSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ElseSyntaxNode > )
         mResult = true;
   }

   void visit( const IfStatmentSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, IfStatmentSyntaxNode > )
         mResult = true;
   }
   void visit( const WhileSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, WhileSyntaxNode > )
         mResult = true;
   }

   void visit( const WhileStatmentSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, WhileStatmentSyntaxNode > )
         mResult = true;
   }

   void visit( const GotoSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, GotoSyntaxNode > )
         mResult = true;
   }

   void visit( const FunctionStatmentSyntaxNodeSP& /*node*/ ) override
   {
      if constexpr( std::is_same_v< T, FunctionStatmentSyntaxNode > )
         mResult = true;
   }
   void visit( const FunctionSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, FunctionSyntaxNode > )
         mResult = true;
   }

   void visit( const FunctionCallSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, FunctionCallSyntaxNode > )
         mResult = true;
   }

   void visit( const ReturnSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ReturnSyntaxNode > )
         mResult = true;
   }

   void visit( const ReturnStatmentSyntaxNodeSP& /* node */ ) override
   {
      if constexpr( std::is_same_v< T, ReturnStatmentSyntaxNode > )
      {

         mResult = true;
      }
   }

   bool result() const
   {
      return mResult;
   }

private:
   bool mResult = false;
};
} // namespace

template< typename T >
static bool check_type( const ISyntaxNodeSP& node )
{
   const auto& visitor = std::make_shared< SyntaxNodeCheckTypeVisitor< T > >();
   node->accept( visitor );
   return visitor->result();
}
    
static std::string to_string( const ISyntaxNodeSP& node )
{
  std::string result;
  std::stringstream s{ result };

  auto print_lexical_tokens = [](const std::vector< LexicalTokens::LexicalToken >& lexical_tokens )
  {
      std::string result;
      std::stringstream s{ result };
      
      for( auto it = lexical_tokens.begin(); it != lexical_tokens.end(); ++it )
      {
         const auto& lexical_token = *it;
         s << lexical_token << ", ";
      }
      return s.str();
  };

  std::unordered_set< ISyntaxNodeSP > visited;

  size_t n = 0;
  iterative_dfs(
     node,
     [ &n, &s, &visited, &print_lexical_tokens ]( const ISyntaxNodeSP& node ) -> bool
     {
        bool visited_node = visited.contains( node );
        if( !visited_node )
        {
           visited.insert( node );
        }
        SyntaxNodeEmptyVisitor::Handlers handlers;
        handlers.bol_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "BOL" << "}"; };
        handlers.plus_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "PLUS" << "}"; };
        handlers.minus_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "MINUS" << "}"; };
        handlers.asterisk_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "ASTERISK" << "}"; };
        handlers.slash_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "SLASH" << "}"; };
        handlers.int_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "INT" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"; };
        handlers.double_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "DOUBLE" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"; };
        handlers.f_syntax_node = [ &s, print_lexical_tokens ]( const FSyntaxNodeSP& node ) { s << "{" << "F" << '(' << node->value() << ')' << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"; };
        handlers.bin_expr_syntax_node = [ &s ]( const BinExprSyntaxNodeSP& node )
        { 
           std::string type;
           switch( node->type() )
           {
           case BinExprSyntaxNode::Type::Addition:
           {
              type = "Addition";
           };
           break;
           case BinExprSyntaxNode::Type::Substruction:
           {
              type = "Substruction";
           };
           break;
           case BinExprSyntaxNode::Type::Multiply:
           {
              type = "Multiply";
           };
           break;
           case BinExprSyntaxNode::Type::Division:
           {
              type = "Division";
           };
           break;
           case BinExprSyntaxNode::Type::Equal:
           {
              type = "Equal";
           };
           break;
           case BinExprSyntaxNode::Type::NotEqual:
           {
              type = "NotEqual";
           };
           break;
           case BinExprSyntaxNode::Type::Less:
           {
              type = "Less";
           };
           break;
           case BinExprSyntaxNode::Type::LessEqual:
           {
              type = "LessEqual";
           };
           break;
           case BinExprSyntaxNode::Type::More:
           {
              type = "More";
           };
           break;
           case BinExprSyntaxNode::Type::MoreEqual:
           {
              type = "MoreEqual";
           };
           break;
           }

           s << "{" << "BIN_EXPR" << '(' << type << ')' << "}";
        };
        handlers.un_expr_syntax_node = [ &s ]( const UnExprSyntaxNodeSP& node )
        { 
           std::string type;
           switch( node->type() )
           {
           case UnExprSyntaxNode::Type::Negation:
           {
              type = "Negation";
           };
           break;
           }

           s << "{" << "UN_EXPR" << '(' << type << ')' << "}";
        };
        handlers.member_expression_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "MEMBER_EXPRESSION" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}" << "}"; };
        handlers.eol_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "EOL" << "}"; };
        handlers.semicolon_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "SEMICOLON" << "}"; };
        handlers.return_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "RETURN" << "}"; };
        handlers.return_statment_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "RETURN_STATMENT" << "}"; };
        handlers.comma_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "COMMA" << "}"; };
        handlers.statment_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "STATMENT" << "}"; };
        handlers.scope_statment_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "SCOPE" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"  << "}"; };
        handlers.function_scope_statment_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "FUNCTION_SCOPE" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"  << "}"; };
        handlers.array_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "ARRAY" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"  << "}"; };
        handlers.object_pair_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "OBJECT_PAIR" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"  << "}"; };
        handlers.object_syntax_node = [ &s, print_lexical_tokens ]( const ISyntaxNodeSP& node ) { s << "{" << "OBJECT" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"  << "}"; };
        handlers.open_curly_bracket_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "OPEN_CURLY_BRACKET" << "}"; };
        handlers.close_curly_bracket_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "CLOSE_CURLY_BRACKET" << "}"; };
        handlers.varible_syntax_node = [ &s ]( const VaribleSyntaxNodeSP& node ) { s << "{" << "VARIBLE" << '(' << node->name() << ')' << "}"; };
        handlers.print_statment_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "PRINT_STATMENT" << "}"; };
        handlers.equal_syntax_node = [ &s ]( const ISyntaxNodeSP& node ) { s << "{" << "EQUAL " << node->lexical_tokens()[0] << "}"; };
        handlers.not_syntax_node = [ &s ]( const ISyntaxNodeSP& node ) { s << "{" << "NOT " << node->lexical_tokens()[0] << "}"; };
        handlers.less_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "LESS" << "}"; };
        handlers.more_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "MORE" << "}"; };
        handlers.if_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "IF" << "}"; };
        handlers.else_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "ELSE" << "}"; };
        handlers.if_statment_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "IF_STATMENT" << "}"; };
        handlers.while_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "WHILE" << "}"; };
        handlers.while_statment_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "WHILE_STATMENT" << "}"; };
        handlers.goto_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "GOTO" << "}"; };
        handlers.function_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "FUNCTION" << "}"; };
        handlers.function_call_syntax_node = [ &s, print_lexical_tokens ]( const FunctionCallSyntaxNodeSP& node ) { s << "{" << "FUNCTION_CALL" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}"   << "}"; };
        handlers.function_statment_syntax_node = [ &s, print_lexical_tokens ]( const FunctionStatmentSyntaxNodeSP& node )
        { s << "{" << "FUNCTION_STATMENT" << " (" << node->name() << ")" << " {" << print_lexical_tokens( node->lexical_tokens() ) << "}" << "}"; };
        handlers.print_syntax_node = [ &s ]( const ISyntaxNodeSP& ) { s << "{" << "PRINT" << "}"; };
        handlers.varible_assigment_statment_syntax_node = [ &s ]( const VaribleAssigmentStatmentSyntaxNodeSP& node )
        {
           std::string context;
           switch( node->context() )
           {
           case VaribleAssigmentStatmentSyntaxNode::Context::GLOBAL:
           {
              context = "GLOBAL";
           };
           break;
           case VaribleAssigmentStatmentSyntaxNode::Context::LOCAL:
           {
              context = "LOCAL";
           };
           break;
           }
           s << "{" << "VARIBLE ASSIGMENT" << " (" << context << ")" << "}";
        };
        handlers.name_syntax_node = [ &s ]( const NameSyntaxNodeSP& node ) { s << "{" << "NAME" << " (" << node->value() << ')' << "}"; };
        handlers.string_syntax_node = [ &s ]( const StringSyntaxNodeSP& node ) { s << "{" << "STRING" << " (" << node->value() << ')' << "}"; };
        for( size_t i = 0; i < n; ++i )
           s << "   ";

        const auto& visitor = std::make_shared< SyntaxNodeEmptyVisitor >( handlers );
        node->accept( visitor );
        s << '\n';

        ++n;
        if( visited_node )
        {
           return true;
        }
        else
        {
           return false;
        }
     },
     [ &n ]( const ISyntaxNodeSP& /* node */ ) { --n; } );
  return s.str();
}

static bool equal( const ISyntaxNodeSP& lhs, const ISyntaxNodeSP& rhs )
{
  bool result = true;
  const auto& our_range = DfsRange< ISyntaxNodeSP >{ std::list< ISyntaxNodeSP >{ lhs } };
  const auto& their_range = DfsRange< ISyntaxNodeSP >{ std::list< ISyntaxNodeSP >{ rhs } };
  for( const auto& [ a, b ] : zip( our_range, their_range ) )
  {
     if( !a || !b )
     {
        result = false;
        break;
     }
     if( *a.value() != *b.value() )
     {
        result = false;
        break;
     }
  }
  return result;
}
