# Mini-C Compiler

A simplified C compiler implementation that demonstrates the key phases of compilation: lexical analysis, syntax analysis, semantic analysis, and intermediate code generation.

## Project Structure

```
mini-c-compiler/
├── src/
│   ├── lexer/         # Lexical analysis using Flex
│   ├── parser/        # Syntax analysis using Bison
│   ├── semantic/      # Semantic analysis
│   ├── codegen/       # Intermediate code generation
│   └── symboltable/   # Symbol table implementation
├── include/           # Header files
├── test/             # Test files
└── examples/         # Example C programs
```

## Prerequisites

- Flex (Fast Lexical Analyzer)
- Bison (Parser Generator)
- GCC/G++ Compiler
- Make

## Building the Project

1. Install the required tools:
   ```bash
   # For Ubuntu/Debian
   sudo apt-get install flex bison build-essential
   # For Windows (using MSYS2/MinGW)
   pacman -S flex bison gcc make
   ```

2. Build the project:
   ```bash
   make
   ```

## Usage

```bash
./mini-c input.c
```

## Features

1. Lexical Analysis
   - Token recognition for C keywords, identifiers, constants, and operators
   - Comment removal
   - Symbol table construction

2. Syntax Analysis
   - Grammar rules for C constructs
   - Error detection with line numbers
   - Support for:
     - Variable declarations
     - Control structures (if, while, for)
     - Functions
     - Arrays
     - Basic expressions

3. Semantic Analysis
   - Type checking
   - Scope resolution
   - Variable declaration verification
   - Function call validation

4. Intermediate Code Generation
   - Three-address code generation
   - Basic optimizations

## Project Status

Under development - See individual components for implementation status. 