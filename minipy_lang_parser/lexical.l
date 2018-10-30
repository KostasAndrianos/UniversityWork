%option noyywrap
%x error
%{
#define YYSTYPE double
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int wrong_words = 0;
void ERROR();
%}

DELIMITER	[ \t\n]+
NONZERO [1-9]
NUMERICAL [0-9]
SIGN [+-]
INT_UNSIGNED (0+|{NONZERO}{NUMERICAL}*)
INT {SIGN}*(0+|{NONZERO}{NUMERICAL}*)
FLOAT {SIGN}*(({INT_UNSIGNED}?\.({NUMERICAL}*([eE]{SIGN}?{NUMERICAL}+)?)))

COMMENT (#.*\n)|(\"{3}(.|\n)*?\"{3})
VARIABLE [a-zA-Z_][a-zA-Z0-9_]*
STRING (\"([^\"\\\n]|\\.)*\")|('([^'\\\n]|\\.)*')
STRING_NOT_CLOSED (\"([^\"\\\n]|\\.)*)|('([^'\\\n]|\\.)*)
STRING_CLOSING_WITH_WRONG_QUOTE_TYPE (\"([^\"\\\n]|\\.)*')|('([^'\\\n]|\\.)*\")
FLOAT_WITH_NO_INT_OR_FRACTION "."
SCIENTIFIC_NOTATION_WITHOUT_OFFSET {SIGN}*{INT_UNSIGNED}?\.({NUMERICAL}*([eE]))
NUMBER_STARTING_WITH_ZERO {SIGN}*(((0+{NONZERO}+)\.({NUMERICAL}*([eE]{SIGN}?{NUMERICAL}+)?))|(0+{NONZERO}+))
VARIABLE_STARTING_WITH_NUMBER {NUMERICAL}+{VARIABLE}
STRING_STARTING_WITH_NUMBER {NUMERICAL}+{STRING}



%%
","                                       { printf("%s", yytext);return ','; }
"r"                                       { printf("%s", yytext);return 'r'; }
"("	                                      { printf("%s", yytext);return '('; }
")"                                       { printf("%s", yytext);return ')'; }
"["                                       { printf("%s", yytext);return '['; }
"]"                                       { printf("%s", yytext);return ']'; }
"="                                       { printf("%s", yytext);return ASSIGN_OP; }
"+"                                       { printf("%s", yytext);return PLUS_OP; }
"-"                                       { printf("%s", yytext);return MINUS_OP; }
"*"                                       { printf("%s", yytext);return MULTI_OP; }
"/"                                       { printf("%s", yytext);return DIV_OP; }
"%"                                       { printf("%s", yytext);return MOD_OP; }
"//"                                      { printf("%s", yytext);return FLOOR_OP; }
"**"                                      { printf("%s", yytext);return EXPON_OP; }
"+="                                      { printf("%s", yytext);return PLUS_AS; }
"-="                                      { printf("%s", yytext);return MINUS_AS; }
"*="                                      { printf("%s", yytext);return MULTI_AS; }
"/="                                      { printf("%s", yytext);return DIV_AS; }
"%="                                      { printf("%s", yytext);return MOD_AS; }
"//="                                     { printf("%s", yytext);return FLOOR_AS; }
"**="                                     { printf("%s", yytext);return EXPON_AS; }
"type"                                    { printf("%s", yytext);return TYPE; }
"len"                                     { printf("%s", yytext);return LEN; }
"input"                                   { printf("%s", yytext);return INPUT; }
"print"                                   { printf("%s", yytext);return PRINT; }
"sep"                                     { printf("%s", yytext);return SEP; }
"end"                                     { printf("%s", yytext);return END; }
\n										                    { yylineno++; return NEWLINE; }
{STRING_STARTING_WITH_NUMBER}             { wrong_words++;ERROR(); BEGIN(error); return STRING_STARTING_WITH_NUMBER; }
{VARIABLE_STARTING_WITH_NUMBER}           { wrong_words++;ERROR(); BEGIN(error); return VARIABLE_STARTING_WITH_NUMBER; }
{FLOAT_WITH_NO_INT_OR_FRACTION}           { wrong_words++;ERROR(); BEGIN(error); return FLOAT_WITH_NO_INT_OR_FRACTION; }
{STRING_CLOSING_WITH_WRONG_QUOTE_TYPE}    { wrong_words++;ERROR(); BEGIN(error); return STRING_CLOSING_WITH_WRONG_QUOTE_TYPE; }
{STRING_NOT_CLOSED}                       { wrong_words++;ERROR(); BEGIN(error); return STRING_NOT_CLOSED; }
{SCIENTIFIC_NOTATION_WITHOUT_OFFSET}      { wrong_words++;ERROR(); BEGIN(error); return SCIENTIFIC_NOTATION_WITHOUT_OFFSET; }
{NUMBER_STARTING_WITH_ZERO}               { wrong_words++;ERROR(); BEGIN(error); return NUMBER_STARTING_WITH_ZERO; }
{COMMENT}							                    { printf("%s", yytext);return COMMENT; }
{STRING}                                  { printf("%s", yytext);return STRING; }
{VARIABLE}						                    { printf("%s", yytext);return VARIABLE; }
{FLOAT}                                   { printf("%s", yytext);return FLOAT; }
{INT}                                     { printf("%s", yytext);return INT; }
{DELIMITER}                               { printf("%s", yytext);}
.                                         { wrong_words++;ERROR("Unknown Error"); BEGIN(error); return TOKEN_ERROR;}
<error>[ \t\n]                            { BEGIN(0); return TOKEN_ERROR; }
<error>.                                  { }
<<EOF>>		{ printf("\n\n\n#FILE ENDED\n#%d line(s) read\n", yylineno-1);
            printf("\n#WRONG TOKENS %d", wrong_words);
            return 0; };
%%


void ERROR() {
        fprintf(yyout, "%s\n", yytext);
}
