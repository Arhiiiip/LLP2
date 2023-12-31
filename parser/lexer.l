%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option noyywrap

%%

"CREATE | create"        { return CREATE; }
"MATCH | match"         { return MATCH; }
"WHERE | where"         { return WHERE; }
"RETURN | return"        { return RETURN; }
"DELETE | delete"        { return DELETE; }
"SET | set"           { return SET; }
"AND | and"           { return AND; }
"OR | or"            { return OR; }
"NOT | not"           { return NOT; }
">"             { return GREATER_CMP; }
">="            { return GREATER_OR_EQUAL_CMP; }
"<"             { return LESS_CMP; }
"<="            { return LESS_OR_EQUAL_CMP; }
"=="            { return EQUAL_CMP; }
"contains"      { return CONTAINS_OP; }
"="             { return ASSIGNMENT; }
"-"             { return DASH; }
"--"            { return DOUBLE_DASH; }
"->"            { return RIGHT_ARROW; }
"<-"            { return LEFT_ARROW; }
":"             { return COLON; }
";"             { return SCOLON; }
"."             { return PERIOD; }
","             { return COMMA; }
"("             { return LPAR; }
")"             { return RPAR; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"true"|"false"  { return BOOL_LITERAL; }
-?[1-9][0-9]*            { yylval.integer = atoi(yytext); return INT_LITERAL         ;}
-?[0-9]+\.[0-9]+         { yylval.real = atof(yytext);    return FLOAT_LITERAL       ;}
\"[^"]*\"                { yylval.string = strdup(yytext);        return STRING_LITERAL      ;}
[A-Za-z][A-Za-z0-9_]*    { yylval.string = strdup(yytext);        return NAME                ;}
<<EOF>>                  { return END_OF_FILE         ;}
[ \t\n]+        ;
\/\/.*\n        ;

%%