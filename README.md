# Compiler Construction Project

A complete compiler for a C-like language, built in 4 phases:

1. **Symbol Table:** Hash chaining with nested scopes.
2. **Lexical Analyzer:** Flex-based tokenization.
3. **Syntax & Semantic Analysis:** Bison parser generating an AST.
4. **Code Generation:** 8086 Assembly output with Peephole Optimization.

## How to Run

1. Ensure you have `g++`, `flex`, and `bison` installed.
2. Run `make` to compile the project.
3. Run `make run` to compile the test program.
4. Check the `output/` folder for the generated Assembly code and Semantic logs.
