# Bytecode-Based VM Interpreter

A two-phase interpreter for the Skrif language that compiles an AST to bytecode and executes it on a stack-based virtual machine.

## How It Works

```
Source Code
  → LexicalTokens   (lexer)
  → AbstractSyntaxTree (LR parser)
  → Compiler        (AST visitor → Chunk)
  → VM              (dispatch loop, operand stack, call frames)
  → Json            (result)
```

The key difference from the tree-walking interpreter is that execution is split into two explicit phases:

1. **Compiler** — walks the AST once and emits a flat sequence of instructions (`Chunk`)
2. **VM** — executes the bytecode via a dispatch loop with no AST traversal at runtime

## Opcodes

| Category | Opcodes |
|----------|---------|
| Literals | `PUSH_NULL`, `PUSH_BOOL`, `PUSH_INT`, `PUSH_DOUBLE`, `PUSH_STRING` |
| Variables | `LOAD`, `STORE_LOCAL`, `STORE_GLOBAL` |
| Arithmetic | `ADD`, `SUB`, `MUL`, `DIV`, `NEG` |
| Comparison | `EQ`, `NEQ`, `LT`, `LTE`, `GT`, `GTE`, `NOT` |
| Collections | `MAKE_ARRAY`, `MAKE_OBJECT`, `GET_MEMBER`, `SET_MEMBER` |
| Control flow | `JUMP`, `JUMP_IF_FALSE` |
| Functions | `MAKE_CLOSURE`, `CALL`, `RETURN` |
| I/O | `PRINT` |
| Stack | `POP` |

## Key Components

- [`Compiler`](include/compiler.h) — `ISyntaxNodeVisitor` that emits bytecode into a `Chunk`
- [`Chunk`](include/chunk.h) — bytecode buffer + constant pools (int, double, string, function protos)
- [`VM`](include/vm.h) — dispatch loop with operand stack (`stack_`) and call frame stack (`frames_`)
- [`Interpreter`](include/interpreter.h) — high-level `eval(string) → Json` facade

## Language Features

All features supported by the tree-walking interpreter are supported here:

- Integers, doubles, strings, booleans, null
- Arithmetic and comparison operators (including mixed int/double)
- Variables: local (`var`) and global scope
- `if` / `else`, `while`
- Arrays and objects with bracket and dot notation
- Functions, closures, recursion, higher-order functions
- `print` statement

## Build

From the repository root:

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## REPL

Start the interactive REPL:

```bash
./project/interpreters/bytecode-based-vm-interpreter/repl/bytecode_vm_interpreter_repl
```

Run a script file:

```bash
./project/interpreters/bytecode-based-vm-interpreter/repl/bytecode_vm_interpreter_repl script.js
```

### Command-line Options

- `--print-ast` - Print the Abstract Syntax Tree of the program
- `--print-bytecode` - Print the compiled bytecode
- `-h, --help` - Show help message

Examples:

```bash
# Print AST of a script
./bytecode_vm_interpreter_repl --print-ast script.js

# Print bytecode of a script
./bytecode_vm_interpreter_repl --print-bytecode script.js

# Print both AST and bytecode
./bytecode_vm_interpreter_repl --print-ast --print-bytecode script.js
```

### REPL Key Bindings

- **Up Arrow** - Navigate to previous history entry
- **Down Arrow** - Navigate to next history entry
- **Ctrl+D** - Exit REPL
- **Ctrl+C** - Exit REPL
- **Enter** - Submit input (or continue to next line if braces are unbalanced)

## Tests

```bash
./project/interpreters/bytecode-based-vm-interpreter/tests/unit_tests/bytecode_vm_interpreter_unit_tests
```

121 unit tests cover arithmetic, variables, control flow, functions, member expressions, and complex scenarios (closures, recursion, higher-order functions).
