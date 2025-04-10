# Mini Compiler for Arithmetic Expressions and Control Statements

## Overview

This project implements a mini compiler using Flex and Yacc in C. The compiler supports arithmetic expressions and basic control statements such as if-else, for, and while. It evaluates arithmetic expressions, checks the validity of input statements, and simulates intermediate code generation by printing the corresponding steps.

## Features

- **Arithmetic Expressions:** Supports addition, subtraction, multiplication, and division.
- **Control Structures:**
  - **If statements:** Evaluate the condition and execute a statement.
  - **If-Else statements:** Provide an alternative execution path when the condition is false.
  - **While loops:** Execute a statement repeatedly while a condition is true.
  - **For loops:** Initialize, test, and update as part of loop control.
- **Feedback Messages:** Displays evaluation results and prints simple intermediate code generation messages.

## Files

- `scanner.l`: The Flex file specifying the lexical analyzer. It tokenizes the input, recognizes numbers, arithmetic operators, control keywords, and other symbols.
- `parser.y`: The Yacc file that defines the grammar for the language of arithmetic expressions and control statements. It includes semantic actions to evaluate expressions and display messages.
- `README.md`: This file.

## Prerequisites

- **Flex:** For generating the lexical analyzer.
- **Yacc or Bison:** For generating the parser.
- **C Compiler:** Such as `gcc` or `cc`.

## Build Instructions

1. Open a terminal and navigate to the project directory.
2. Generate the lexer using Flex:
   ```bash
   flex scanner.l
   ```
3. Generate the parser using Yacc (or Bison):
   ```bash
   yacc -d parser.y
   ```
4. Compile the generated files:
   ```bash
   cc lex.yy.c y.tab.c -o mini_compiler
   ```

## Running the Compiler

Start the compiler by running the executable:

```bash
./mini_compiler
```

You will be prompted to enter statements. Each statement should end with a semicolon (`;`). For example:

```c
3+4;
if(7-2) 3+2;
if(1) 5+5; else 4+4;
while(2) 3+3;
for(1; 2; 3) 1+2;
```

## Expected Output

- For an arithmetic expression like `3+4;`, you might see:
  ```
  Intermediate Code: 3 + 4 = 7
  Arithmetic expression evaluated to: 7
  ```
- For control structures, the output will include the condition value and a description of the structure executed. For example:
  ```
  If statement: condition evaluated to 5
  ```

## Extending the Project

This mini compiler provides a basic framework that you can extend further by:

- Enhancing the grammar to support more complex expressions.
- Implementing a full symbol table for variables.
- Generating complete three-address intermediate code.
- Adding error detection and recovery for invalid statements.

Feel free to contribute and improve the project as needed!
