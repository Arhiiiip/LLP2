%{
#include "ast.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

extern int yylex(void);
extern void yyerror(const char *s);
%}

%union {
    struct Expr *expr;
    int integer;
    float real;
    bool boolean;
    char *string;
    char *name;
}

%token
    CREATE
    MATCH
    WHERE
    RETURN
    DELETE
    SET
    AND
    OR
    NOT
    GREATER_CMP
    GREATER_OR_EQUAL_CMP
    LESS_CMP
    LESS_OR_EQUAL_CMP
    EQUAL_CMP
    CONTAINS_OP
    ASSIGNMENT
    DASH
    DOUBLE_DASH
    RIGHT_ARROW
    LEFT_ARROW
    COLON
    SCOLON
    PERIOD
    COMMA
    LPAR
    RPAR
    LBRACKET
    RBRACKET
    LBRACE
    RBRACE
    END_OF_FILE
    BOOL_LITERAL
    INT_LITERAL
    FLOAT_LITERAL
    STRING_LITERAL
    NAME

%type <expr> REQUEST REQUEST_B MATCH_EXPRESSION VARIABLE_MATCH RELATION_MATCH ANY_RELATION_MATCH
    RETURN_EXPRESSION VALUE CREATE_EXPRESSION SET_EXPRESSION DELETE_EXPRESSION FILTER PREDICATE
    LOGICAL_EXPRESSION ATTRIBUTE_LIST

%type <string> STRING_LITERAL
%type <integer> INT_LITERAL
%type <real> FLOAT_LITERAL
%type <boolean> BOOL_LITERAL
%type <name> NAME

%left OR AND NOT
%left EQUAL_CMP GREATER_CMP GREATER_OR_EQUAL_CMP LESS_CMP LESS_OR_EQUAL_CMP CONTAINS_OP

%start REQUEST

%%

REQUEST: REQUEST_B SCOLON                { $$ = $1; }
       | REQUEST_B END_OF_FILE          { $$ = $1; }
;

REQUEST_B: MATCH_EXPRESSION              { $$ = create_request_node($1); }
         | CREATE_EXPRESSION             { $$ = create_request_node($1); }
         | REQUEST_B MATCH_EXPRESSION    { $$ = append_to_request($1, $2); }
         | REQUEST_B SET_EXPRESSION       { $$ = append_to_request($1, $2); }
         | REQUEST_B CREATE_EXPRESSION    { $$ = append_to_request($1, $2); }
         | REQUEST_B DELETE_EXPRESSION    { $$ = append_to_request($1, $2); }
         | REQUEST_B RETURN_EXPRESSION    { $$ = append_to_request($1, $2); }
;

CREATE_EXPRESSION: CREATE VARIABLE_MATCH { $$ = create_create_expression($2); }
                | CREATE VARIABLE_MATCH RELATION_MATCH VARIABLE_MATCH { $$ = create_create_expression_with_relation($2, $3, $4); }
                | CREATE VARIABLE_MATCH ANY_RELATION_MATCH VARIABLE_MATCH { $$ = create_create_expression_with_any_relation($2, $4); }
                // Добавьте другие варианты, если необходимо
;

MATCH_EXPRESSION: MATCH VARIABLE_MATCH                                { $$ = create_match_expression($2, NULL, NULL); }
                | MATCH VARIABLE_MATCH RELATION_MATCH VARIABLE_MATCH { $$ = create_match_expression($2, $3, $4); }
                | MATCH VARIABLE_MATCH ANY_RELATION_MATCH VARIABLE_MATCH { $$ = create_match_expression($2, NULL, $4); }
;

VARIABLE_MATCH: LPAR NAME COLON NAME PREDICATE RPAR                   { $$ = create_variable_filter_match($2, $4, $5);  }
              | LPAR NAME COLON NAME LBRACE ATTRIBUTE_LIST RBRACE RPAR { $$ = create_variable_pattern_match($2, $4, $6); }
              | LPAR NAME COLON NAME RPAR                              { $$ = create_variable_match($2, $4); }
              | LPAR NAME RPAR                                         { $$ = create_variable_match($2, NULL); }
;

RELATION_MATCH: DASH LBRACKET NAME COLON NAME RBRACKET RIGHT_ARROW   { $$ = create_relation_match($3, $5, FORWARD); }
              | LEFT_ARROW LBRACKET NAME COLON NAME RBRACKET DASH    { $$ = create_relation_match($3, $5, REVERSE); }
;

ANY_RELATION_MATCH: DOUBLE_DASH { $$ = create_any_relation_match(); }
;

PREDICATE: WHERE LOGICAL_EXPRESSION { $$ = create_predicate($2); }
;

LOGICAL_EXPRESSION: LOGICAL_EXPRESSION AND LOGICAL_EXPRESSION       { $$ = create_logical_and_operation($1, $3); }
                  | LOGICAL_EXPRESSION OR LOGICAL_EXPRESSION        { $$ = create_logical_or_operation($1, $3); }
                  | NOT LOGICAL_EXPRESSION                         { $$ = create_logical_not_operation($2); }
                  | FILTER                                       { $$ = create_filter_bypass($1); }
;

FILTER: VALUE LESS_CMP VALUE             { $$ = create_filter($1, $3, LESS); }
      | VALUE LESS_OR_EQUAL_CMP VALUE    { $$ = create_filter($1, $3, LESS_OR_EQUAL); }
      | VALUE GREATER_CMP VALUE          { $$ = create_filter($1, $3, GREATER); }
      | VALUE GREATER_OR_EQUAL_CMP VALUE { $$ = create_filter($1, $3, GREATER_OR_EQUAL); }
      | VALUE EQUAL_CMP VALUE            { $$ = create_filter($1, $3, EQUAL); }
      | VALUE CONTAINS_OP VALUE          { $$ = create_filter($1, $3, CONTAINS); }
;

SET_EXPRESSION: SET NAME PERIOD NAME ASSIGNMENT VALUE { $$ = create_set_expression(create_variable_value($2, $4), $6); }
;

DELETE_EXPRESSION: DELETE NAME { $$ = create_delete_expression($2); }
;

RETURN_EXPRESSION: RETURN_EXPRESSION COMMA VALUE { $$ = append_to_return_expression($1, $3); }
                 | RETURN VALUE                 { $$ = create_return_expression($2); }
;

VALUE: NAME      { $$ = create_variable_value(NULL, $1); }
     | STRING_LITERAL { $$ = create_string_value($1); }
     | INT_LITERAL { $$ = create_int_value($1); }
     | FLOAT_LITERAL { $$ = create_float_value($1); }
     | BOOL_LITERAL { $$ = create_bool_value($1); }
     | MATCH_EXPRESSION { $$ = create_subquery_value($1); }
     | LPAR LOGICAL_EXPRESSION RPAR { $$ = create_logical_expression_value($2); }
;

ATTRIBUTE_LIST: ATTRIBUTE_LIST COMMA NAME { $$ = append_to_attribute_list($1, $3); }
              | NAME                     { $$ = create_attribute_list($1); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}
