#include "interpreter.h"
#include "compiler.h"
#include "vm.h"
#include "EnvScope.h"
#include "lexical_tokens.h"
#include "abstract_syntax_tree.h"

Json Interpreter::eval(const std::string& expression) const
{
    LexicalTokens lexical_tokens(expression);
    AbstractSyntaxTree ast(lexical_tokens);

    auto compiler = std::make_shared<Compiler>();
    Chunk chunk = compiler->compile(ast.root());

    auto global_env = std::make_shared<EnvScope>();
    VM vm;
    return vm.run(chunk, global_env);
}
