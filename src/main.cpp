#include <iostream>
#include <fstream>
#include "parser/parser.tab.h"
#include "parser/ast.h"
#include "symbol_table/SymbolTable.h"
#include "parser/codegen.h"

extern FILE* yyin;
extern ProgramNode* rootAST;
extern SymbolTable* symTable;
extern FILE* logFile;

int main(int argc, char** argv) {
    if (argc < 2) { std::cerr << "Usage: ./compiler <input_file>" << std::endl; return 1; }
    
    logFile = fopen("output/semantic_log.txt", "w");
    symTable = new SymbolTable(7, logFile);
    
    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) { std::cerr << "Error: Could not open input file." << std::endl; return 1; }
    yyin = inputFile;

    std::cout << "=== COMPILER STARTED ===" << std::endl;
    std::cout << "Phase 1 & 2: Lexical & Syntax Analysis..." << std::endl;
    int parseResult = yyparse();

    if (parseResult == 0) {
        std::cout << "\nPhase 3: Semantic Analysis & AST Generation Successful!" << std::endl;
        std::cout << "Generating Parse Tree:\n";
        if (rootAST) rootAST->print(0);

        std::cout << "\nPhase 4: 8086 Code Generation & Peephole Optimization..." << std::endl;
        CodeGenerator codeGen("output/output.asm");
        codeGen.generate(rootAST);
        std::cout << "Assembly code saved to output/output.asm" << std::endl;
    } else {
        std::cout << "\nCompilation Failed due to Syntax or Semantic Errors." << std::endl;
    }

    fclose(inputFile);
    fclose(logFile);
    delete symTable;
    return parseResult;
}