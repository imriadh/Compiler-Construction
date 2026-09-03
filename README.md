
# 🛠️ Compiler Construction Project

A complete, multi-phase compiler for a C-like programming language, built from scratch using C++, Flex, and Bison. This project implements the entire frontend and backend of a compiler, from lexical analysis to 8086 assembly code generation.

## 🚀 Project Phases

This compiler is divided into four distinct phases, corresponding to the core assignments of the Compiler Construction course:

### Phase 1: Symbol Table Implementation
- **Hash Chaining:** Uses the djb2 hash algorithm for efficient O(1) average-case lookups.
- **Nested Scopes:** Supports hierarchical scopes (e.g., `1`, `1.1`, `1.2`) using a parent-pointer structure.
- **Operations:** Insertion, deletion, lookup, and scope management (Enter/Exit).

### Phase 2: Lexical Analysis
- **Tool:** Flex (Fast Lexical Analyzer Generator).
- **Functionality:** Tokenizes C-like source code into a structured stream of tokens (Keywords, Identifiers, Literals, Operators).
- **Features:** Tracks line/character numbers and ignores comments/whitespace.

### Phase 3: Syntax & Semantic Analysis
- **Tool:** Bison (GNU Parser Generator).
- **Abstract Syntax Tree (AST):** Builds a polymorphic C++ AST representing the logical structure of the code.
- **Semantic Checking:** Integrates with the Symbol Table to catch undeclared variables and duplicate declarations at compile time.

### Phase 4: Code Generation & Optimization
- **Target Architecture:** Intel 8086 Microprocessor.
- **Output:** Generates valid `.asm` assembly code with proper `.DATA` and `.CODE` segments.
- **Optimization:** Implements a **Peephole Optimizer** to eliminate redundant instructions (e.g., useless `PUSH`/`POP` pairs) for faster execution.

## 📁 Project Structure

```text
Compiler-Construction/
── src/
│   ├── symbol_table/      # Phase 1: SymbolInfo, ScopeTable, SymbolTable
│   ├── lexer/             # Phase 2: Flex lexical analyzer (lexer.l)
│   ├── parser/            # Phase 3 & 4: Bison grammar, AST, Code Generator
│   └── main.cpp           # Driver code tying all phases together
├── test/
│   └── test.c             # Sample C-like input code
├── output/                # Generated Assembly and Semantic Logs
├── Makefile               # Automation for building and running
└── README.md
```

## ️ Prerequisites

To build and run this project, you need the following tools installed on your system (Linux, macOS, or Windows via WSL/MSYS2):
- `g++` (C++ Compiler)
- `flex` (Lexical Analyzer Generator)
- `bison` (Parser Generator)
- `make` (Build Automation Tool)

## ▶️ How to Run

1. **Clone the repository:**
   ```bash
   git clone https://github.com/your-username/Compiler-Construction.git
   cd Compiler-Construction
   ```

2. **Compile the project:**
   ```bash
   make
   ```

3. **Run the compiler on the test file:**
   ```bash
   make run
   ```

4. **Clean generated files:**
   ```bash
   make clean
   ```

##  Example Usage

**Input (`test/test.c`):**
```c
int x = 5;
int y = 10;
int z = x + y;
```

**Generated 8086 Assembly (`output/output.asm`):**
```assembly
.MODEL SMALL
.STACK 100H
.DATA
x DW 0
y DW 0
z DW 0
.CODE
MAIN PROC
    MOV AX, @DATA
    MOV DS, AX
    MOV AX, 5
    MOV [x], AX
    MOV AX, 10
    MOV [y], AX
    MOV AX, [x]
    PUSH AX
    MOV BX, [y]
    POP AX
    ADD AX, BX
    MOV [z], AX
    MOV AH, 4CH
    INT 21H
MAIN ENDP
END MAIN
```

##  References
- Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D. (2007). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson Education.
- Levine, J. R. (2009). *Flex & Bison: Text Processing Tools*. O'Reilly Media.

## 👤 Author
**Student Name:** Riad Hossain 
**Course:** Compiler Construction  
**Department:** Department of Computer Science and Engineering
