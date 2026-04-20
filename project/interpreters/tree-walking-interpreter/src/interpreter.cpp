#include "interpreter.h"

#include "stack_machine.h"
#include "lexical_tokens.h"
#include "abstract_syntax_tree.h"
#include "control_flow_graph.h"

Json Interpreter::eval( const std::string& expression ) const
{
    LexicalTokens lexical_tokens( expression );
    AbstractSyntaxTree ast( lexical_tokens );
    ControlFlowGraph cfg{ ast };
    StackMachine stack_machine{ cfg };
    return stack_machine.exec();
}
