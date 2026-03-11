#include "interpreter.h"

#include "stack_machine.h"
#include "lexical_tokens.h"
#include "abstract_syntax_tree.h"
#include "control_flow_graph.h"
#include <exception>
#include <iostream>
#include <string>

   // // Fibonacci number in sequence
   // // 23's number shall be 17711
   // std::string e = R"(
   //  {
   //    a = 1+(2+3)+(4*5)/2;
   //
   //    if( 2 == 2 )
   //    {
   //      1+(2+3)+(4*5)/2;
   //      a = 232 + 2;  
   //    }
   //    function fibonacci(n)
   //    {
   //      a = 0;
   //      b = 1;
   //      i = 2;
   //      while( i < n )
   //      {
   //          // c = a + b;
   //          a = b;
   //          b = c;
   //          // i = i + 1;
   //      }
   //      return b;
   //    }
   //    // number = fibonacci(23);
   //    print(number);
   //    print(number==17711);
   //
   //    // r_fibonacci(n)
   //    // {
   //    //   // print(n*10);
   //    //   // print( n );
   //    //   if( n == 0 )
   //    //   {
   //    //     // print( 0 );
   //    //     return 0;
   //    //   }
   //    //   if( n == 1 )
   //    //   {
   //    //     // print( 1 );
   //    //     return 1;
   //    //   }
   //    //   a = r_fibonacci(n-2);
   //    //   print(a);
   //    //   b = r_fibonacci(n-1);
   //    //   print(b);
   //    //   // print( a + b);
   //    //   return a + b;
   //    // }
   //    // number = r_fibonacci(3);
   //    print(number);
   //    print(number==17711);
   //    // print( r_fibonacci(0) );
   //    // print( r_fibonacci(1) );
   //    // print( r_fibonacci(2) );
   //   // r_fibonacci(3);
   //    // print( r_fibonacci(3) );
   //  })";
   // //
   // //        if(3 < 2)
   // //        {
   // //            print(123);
   // //
   // //            3+5;
   // //        }
   // //        print(456);
   // //
   // //        2 < 3;
   // //        2 > 3;
   // //        -1+3;
   // //        {
   // //            2+2;
   // //        }
   // //    std::string e = "((-1+2)-3)";
   // //    std::string e = "{a=5+5;b=10+17;}";
   //
   // //    int n = 5;
   // //    for (int i=1; i<n; ++i)
   // //    {
   // //        if( i % 2 == 0 )
   // //        {
   // //            e += std::to_string(i)+"+";
   // //        }
   // //        else
   // //        {
   // //            e += std::to_string(i)+"*";
   // //        }
   // //    }
   // //    e += std::to_string(n);
   // std::cout << e << std::endl;
   //
   // Calculator calculator;
   // auto result0 = calculator.solve( e );
   // cout << result0 << std::endl;
   // return 0;

Json Interpreter::eval( const std::string& expression ) const
{
  Json result;
  // try 
  // {
    // std::cout << "== Lexemes ==" << std::endl;
    LexicalTokens lexical_tokens( expression );
    // std::cout << lexical_tokens << std::endl;

    std::cout << "== AST ==" << std::endl;
    AbstractSyntaxTree ast( lexical_tokens );
    std::cout << ast << std::endl;

    std::cout << "== CFG ==" << std::endl;
    ControlFlowGraph cfg{ ast };
    std::cout << cfg << std::endl;

    std::cout << "== Executioning ==" << std::endl;
    StackMachine stack_machine{ cfg };
    result = stack_machine.exec();
  // } 
  // catch (const std::exception& e)
  // {
  //   std::cerr << e.what() << std::endl;
  // } 
  
  return result;
}
