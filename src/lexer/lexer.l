%{
#include "../parser/parser.tab.h"
#include <string>
#include <cstring>
%}

%option noyywrap nounput noinput

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ALPHANUM    [a-zA-Z0-9_]
WHITESPACE  [ \t\r\n]+

%%
{WHITESPACE}    { /* ignore whitespace */ }
"//".*          { /* ignore single line comments */ }

"int"           { return INT; }
"float"         { return FLOAT; }
"double"        { return DOUBLE; }

{LETTER}{ALPHANUM}*   { yylval.sval = strdup(yytext); return ID; }
{DIGIT}+\.{DIGIT}+    { yylval.fval = atof(yytext); return FLOAT_LIT; }
{DIGIT}+              { yylval.ival = atoi(yytext); return INT_LIT; }

"+"           { return '+'; }
"-"           { return '-'; }
"*"           { return '*'; }
"/"           { return '/'; }
"="           { return '='; }
";"           { return ';'; }

.             { fprintf(stderr, "Lexer Error: Unexpected character '%s'\n", yytext); }
%%