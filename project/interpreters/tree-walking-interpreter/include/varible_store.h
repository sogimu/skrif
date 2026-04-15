#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <ostream>
#include <string>

#include "json.h"

class VaribleStore
{
public:
   void pushScope()
   {
      mScope.emplace_back( std::unordered_map< std::string, Json >{} );
   }

   void popScope()
   {
      mScope.pop_back();
   }

   Json& operator[]( const std::string& name )
   {
      auto it = std::find_if( mScope.rbegin(), mScope.rend(),
                              [ &name ]( const auto& layer ) -> bool
                              {
                                 const auto& it = layer.find( name );
                                 return ( it != layer.end() );
                              } );
      if( it != mScope.rend() )
      {
         return ( *it )[ name ];
      }
      return mScope.back()[ name ];
   }

   const Json& operator[]( const std::string& name ) const
   {
      auto it = std::find_if( mScope.rbegin(), mScope.rend(),
                              [ &name ]( const auto& layer ) -> bool
                              {
                                 const auto& it = layer.find( name );
                                 return ( it != layer.end() );
                              } );
      if( it != mScope.rend() )
      {
         return ( *it )[ name ];
      }
      return mScope.back()[ name ];
   }

   void writeValueToLocalVarible( const std::string& name, const Json& value )
   {
      mScope.back()[ name ] = value;
   }

   void print() const
   {
      std::cout << "*************" << std::endl;
      int index = mScope.size();
      for( auto it = mScope.rbegin(); it != mScope.rend(); ++it )
      {
         const auto& layer = *it;
         std::cout << "=LAYER (" << std::to_string( index ) << "): ";
         for( const auto& [ key, value ] : layer )
         {
            std::cout << key << " == " << value << ", ";
         }
         --index;
         std::cout << std::endl;
      }
      std::cout << "*************" << std::endl;
   }

private:
   mutable std::deque< std::unordered_map< std::string, Json > > mScope;
};
