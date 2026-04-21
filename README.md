# Skrif

> A JavaScript-like interpreter built on a Shift-Reduce (LR) parser, written in C++

[![Workflow Status](https://github.com/sogimu/skrif/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/sogimu/skrif/actions/workflows/cmake-single-platform.yml)

## Performance vs V8

`Release` build · Node.js v18.20.8 · startup overhead subtracted from V8 times

| Benchmark | V8 (net) | Bytecode VM | Tree-Walking |
|---|---|---|---|
| closures.js | 6 ms | 7.7× slower | **517× slower** |
| fib_iterative.js | 6 ms | 8.2× slower | **895× slower** |
| fib_recursive.js | 7 ms | 9.4× slower | **665× slower** |
| nested_loops.js | 6 ms | 8.2× slower | **628× slower** |

Run benchmarks: `bash benchmarks/run.sh`

## Overview

This project is an educational implementation of a JavaScript-like interpreter using LR (bottom-up) parsing. Started as a learning exercise, it demonstrates core compiler construction concepts and may evolve into a more comprehensive language implementation.

## Features

- **LR Parser (Bottom-Up)** — efficient Shift-Reduce algorithm for syntax analysis
- **Full Interpreter** — real-time execution of JavaScript-like code  
- **Rich Syntax** — support for arithmetic, conditionals, functions, objects and arrays
- **Comprehensive Testing** — 200+ unit tests for reliability
- **REPL Interface** — interactive console for quick testing
- **Modular Architecture** — clean separation of parser and interpreter

## Quick Start

### Requirements
- **C++20** compatible compiler (GCC 10+, Clang 10+, MSVC 19.29+)
- **CMake 3.6+**
- **libreadline-dev** (Linux only, for REPL)

### Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libreadline-dev
```

**macOS:**
```bash
brew install cmake readline
```

### Build

```bash
git clone https://github.com/sogimu/skrif.git
cd skrif
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Run REPL

Tree-walking interpreter:
```bash
./project/interpreters/tree-walking-interpreter/repl/repl
```

Bytecode VM interpreter:
```bash
./project/interpreters/bytecode-based-vm-interpreter/repl/bytecode_vm_interpreter_repl
```

## Examples

### Basic Arithmetic
```javascript
> 2 + 3 * 4;
14

> (5 - 2) / 3;
1
```

### Variables
```javascript
> { var x = 42;
  var y = x * 2;
  print(y);
}
84
```

### Conditionals
```javascript
> if (x > 40) {
    print("x is large");
  };
x is large
```

### Functions
```javascript
> { function add(a, b) {
    return a + b;
  };
  add(10, 20); }
30
```

### Objects and Arrays
```javascript
> { var arr = [1, 2, 3];
  print(arr[0]); }
1

> { var obj = {"name": "John", "age": 30};
  print(obj.name); }
"John"
```

## Architecture

```
project/
├── parser/                          # LR parser and AST
├── interpreters/                    # Execution engines
│   ├── tree-walking-interpreter/    # Direct AST traversal
│   └── bytecode-based-vm-interpreter/ # Compile to bytecode, then execute
└── libs/                           # External libraries (GoogleTest)
```

Two interpreter implementations share the same parser and language semantics:

### Tree-Walking Interpreter

Executes by walking the AST directly on each evaluation. Simple and straightforward.

- [`StackMachine`](project/interpreters/tree-walking-interpreter/include/stack_machine.h) — AST visitor that evaluates expressions
- [`Interpreter`](project/interpreters/tree-walking-interpreter/include/interpreter.h) — high-level `eval()` interface

### Bytecode VM Interpreter

Two-phase execution: compile AST to bytecode once, then run in a dispatch loop.

- [`Compiler`](project/interpreters/bytecode-based-vm-interpreter/include/compiler.h) — AST visitor that emits bytecode
- [`Chunk`](project/interpreters/bytecode-based-vm-interpreter/include/chunk.h) — bytecode buffer and constant pools
- [`VM`](project/interpreters/bytecode-based-vm-interpreter/include/vm.h) — stack-based dispatch loop
- [`Interpreter`](project/interpreters/bytecode-based-vm-interpreter/include/interpreter.h) — high-level `eval()` interface

### Shared Components

- [`LexicalTokens`](project/parser/include/lexical_tokens/lexical_tokens.h) — lexical analyzer
- [`AbstractSyntaxTree`](project/parser/include/abstract_syntax_tree/abstract_syntax_tree.h) — AST construction (LR parser)

## Testing

### Run All Tests
```bash
cd build
ctest --verbose
```

### Run Specific Tests
```bash
# Parser tests
./project/parser/tests/unit_tests/parser_tests

# Tree-walking interpreter tests
./project/interpreters/tree-walking-interpreter/tests/unit_tests/tree_walking_interpreter_unit_tests

# Bytecode VM interpreter tests
./project/interpreters/bytecode-based-vm-interpreter/tests/unit_tests/bytecode_vm_interpreter_unit_tests
```

## Roadmap
- [x] **Numbers**
- [x] **Strings**
- [x] **Arithmetic operations**
- [x] **Control flow statments**
- [x] **Variables**
- [x] **Array**
- [x] **Object**
- [x] **Function call** Standart notation
- [x] **Function call** Member notation
- [x] **Member expresson** Bracket notation
- [x] **Member expresson** dot notation
- [ ] **Standart library** Console object
- [ ] **Standard library** Math
- [x] **Function closure**
- [ ] **Module system** (import/export)
- [ ] **Test262** compliance testing
- [ ] **Debugger** with breakpoints

## License

This project is licensed under the MIT License. See [`LICENSE`](LICENSE) for details.

---

*Built as an educational project with potential for serious development*
