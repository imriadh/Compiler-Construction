CC = g++
CFLAGS = -std=c++11 -w

all: compiler

compiler: src/lexer/lexer.l src/parser/parser.y src/parser/ast.h src/parser/codegen.h src/symbol_table/SymbolInfo.h src/symbol_table/ScopeTable.h src/symbol_table/SymbolTable.h src/main.cpp
	bison -d src/parser/parser.y -o src/parser/parser.tab.c
	flex -o src/lexer/lex.yy.c src/lexer/lexer.l
	$(CC) $(CFLAGS) src/parser/parser.tab.c src/lexer/lex.yy.c src/main.cpp -o compiler -lfl

run: compiler
	./compiler test/test.c

clean:
	rm -f compiler src/parser/parser.tab.c src/parser/parser.tab.h src/lexer/lex.yy.c output/semantic_log.txt output/output.asm