# Shift-Reduce Parser & JavaScript Interpreter

> Parser and interpreter for JavaScript-like language built on Shift-Reduce algorithm

[![Workflow Status](https://github.com/sogimu/shift-reduce-parser/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/sogimu/shift-reduce-parser/actions/workflows/cmake-single-platform.yml)

## Overview

This project is an educational implementation of a JavaScript-like interpreter using LR (bottom-up) parsing. Started as a learning exercise, it demonstrates core compiler construction concepts and may evolve into a more comprehensive language implementation.

## Features

- **LR Parser (Bottom-Up)** — efficient Shift-Reduce algorithm for syntax analysis
- **Full Interpreter** — real-time execution of JavaScript-like code  
- **Rich Syntax** — support for arithmetic, conditionals, functions, objects and arrays
- **Comprehensive Testing** — 100+ unit tests for reliability
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
git clone https://github.com/sogimu/shift-reduce-parser.git
cd shift-reduce-parser
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Run REPL

```bash
./project/interpreters/naive_stack_interpreter/repl/repl
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
├── parser/           # LR parser and AST
├── interpreters/     # Execution engines
│   └── naive_stack_interpreter/
└── libs/            # External libraries (GoogleTest)
```

### Key Components:

- [`LexicalTokens`](project/parser/include/lexical_tokens/lexical_tokens.h) — lexical analyzer
- [`AbstractSyntaxTree`](project/parser/include/abstract_syntax_tree/abstract_syntax_tree.h) — AST construction
- [`StackMachine`](project/interpreters/naive_stack_interpreter/include/stack_machine.h) — stack-based interpreter
- [`Interpreter`](project/interpreters/naive_stack_interpreter/include/interpreter.h) — high-level interface

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

# Interpreter tests  
./project/interpreters/naive_stack_interpreter/tests/unit_tests/interpreter_tests
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
- [ ] **Function call** Member notation
- [x] **Member expresson** Bracket notation
- [ ] **Member expresson** dot notation
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
