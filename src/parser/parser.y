%code requires {
    #include <string>
    #include <vector>
    #include "ast.h"
}

%{
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ast.h" /* Added this so global variables can see ProgramNode */
#include "../symbol_table/SymbolTable.h"

ProgramNode* rootAST = nullptr;
SymbolTable* symTable = nullptr;
FILE* logFile = nullptr;

void yyerror(const char *s);
int yylex();
%}

/* Define the data types our grammar rules can return */
%union { 
    int ival; 
    double fval; 
    char* sval; 
    ASTNode* node; 
    std::vector<ASTNode*>* vec; 
}

%token INT FLOAT DOUBLE
%token <ival> INT_LIT
%token <fval> FLOAT_LIT
%token <sval> ID
%token '=' ';' '+' '-' '*' '/'

/* Map grammar rules to their correct C++ data types */
%type <node> program declaration expression term factor
%type <vec> declaration_list
%type <sval> type_specifier

%%

program:
    declaration_list { 
        rootAST = new ProgramNode(); 
        rootAST->statements = *$1; 
        delete $1; 
    }
    ;

declaration_list:
    declaration { 
        $$ = new std::vector<ASTNode*>(); 
        $$->push_back($1); 
    }
    | declaration_list declaration { 
        $1->push_back($2); 
        $$ = $1; 
    }
    ;

declaration:
    type_specifier ID ';' {
        SymbolInfo* sym = new SymbolInfo(std::string($2), std::string($1), std::string($1), "VARIABLE");
        if(!symTable->insert(sym)) { 
            fprintf(stderr, "Semantic Error: Variable '%s' already declared.\n", $2); 
            delete sym; 
        }
        $$ = new DeclarationNode(std::string($1), std::string($2));
        free($2); free($1);
    }
    | type_specifier ID '=' expression ';' {
        SymbolInfo* sym = new SymbolInfo(std::string($2), std::string($1), std::string($1), "VARIABLE");
        symTable->insert(sym);
        $$ = new AssignmentNode(std::string($2), $4);
        free($2); free($1);
    }
    ;

type_specifier:
    INT    { $$ = strdup("int"); }
    | FLOAT  { $$ = strdup("float"); }
    | DOUBLE { $$ = strdup("double"); }
    ;

expression:
    term { $$ = $1; }
    | expression '+' term { $$ = new BinaryOpNode('+', $1, $3); }
    | expression '-' term { $$ = new BinaryOpNode('-', $1, $3); }
    ;

term:
    factor { $$ = $1; }
    | term '*' factor { $$ = new BinaryOpNode('*', $1, $3); }
    | term '/' factor { $$ = new BinaryOpNode('/', $1, $3); }
    ;

factor:
    INT_LIT   { $$ = new NumberNode($1); }
    | FLOAT_LIT { $$ = new NumberNode($1); }
    | ID      { 
        if(symTable->lookup($1) == nullptr) {
            fprintf(stderr, "Semantic Error: Undeclared identifier '%s'\n", $1);
        }
        $$ = new IdentifierNode($1); 
        free($1); 
    }
    ;

%%

void yyerror(const char *s) { 
    fprintf(stderr, "Syntax Error: %s\n", s); 
}