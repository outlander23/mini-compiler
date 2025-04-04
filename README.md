# Mini Compiler for Arithmetic Expressions and Control Statements

This project implements a mini compiler that processes arithmetic expressions and control statements (If-Else, For, While) using Flex and Yacc.

## Project Structure

- **src/**: Contains the source code files.
  - **lexer.l**: Lexical analyzer definitions.
  - **parser.y**: Grammar rules and parsing logic.
  - **main.c**: Entry point of the compiler.
  - **symtab.h**: Header for symbol table management.
  - **symtab.c**: Implementation of symbol table functions.
  - **ast.h**: Header for abstract syntax tree structures.
  - **ast.c**: Implementation of AST manipulation functions.
  - **codegen.h**: Header for code generation functions.
  - **codegen.c**: Implementation of code generation logic.

- **test/**: Contains test cases for validating compiler functionality.
  - **arith.txt**: Test cases for arithmetic expressions.
  - **if_else.txt**: Test cases for if-else statements.
  - **for.txt**: Test cases for for loops.
  - **while.txt**: Test cases for while loops.

- **Makefile**: Build instructions for compiling the project.

## Setup Instructions

1. Ensure you have Flex and Yacc installed on your system.
2. Clone the repository or download the project files.
3. Navigate to the project directory.
4. Run `make` to build the project.

## Usage Guidelines

To run the compiler, execute the compiled binary with a source file containing arithmetic expressions or control statements. The compiler will validate the input, evaluate expressions, and generate intermediate code.

## Testing

Test the compiler using the provided test cases in the `test` directory. Each file contains specific scenarios to validate the functionality of the compiler.