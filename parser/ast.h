// ast.h
#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef struct Expr {
    enum {
        INT_EXPR,
        FLOAT_EXPR,
        STRING_EXPR,
        BOOL_EXPR,
        NAME_EXPR,
        RELATION_EXPR,
        PROPERTY_EXPR,
        MATCH_EXPR,
        CREATE_EXPR,
        DELETE_EXPR,
        SET_EXPR,
        LIST_EXPR,
        STATEMENT_LIST
    } type;
    union {
        int int_val;
        float float_val;
        char* str_val;
        bool boolean;
        struct {
            struct Expr* left;
            struct Expr* right;
            enum RelationType{
                ARROW_RELATION,
                DASH_RELATION
            } rel_type;
        } relation_expr;
        struct {
            struct Expr* object;
            struct Expr* property;
        } property_expr;
        struct {
            struct Expr* pattern;
            struct Expr* condition;
            struct Expr* ret_expr;
        } match_expr;
        struct {
            struct Expr* pattern;
        } create_expr;
        struct {
            struct Expr* pattern;
        } delete_expr;
        struct {
            struct Expr* expr;
            struct Expr* next;
        } expr_list;
        struct {
            struct Expr* statement;
            struct Expr* next;
        } statement_list;
        struct {
            struct Expr* target;
            struct Expr* value;
        } set_expr;
    } value;
} Expr;

Expr* create_int_expr(int value);
Expr* create_float_expr(float value);
Expr* create_string_expr(const char* value);
Expr* create_bool_expr(int value);
Expr* create_name_expr(const char* name);
Expr* create_relation_expr(Expr* left, Expr* right, enum RelationType rel_type);
Expr* create_property_expr(Expr* object, Expr* property);
Expr* create_match_expr(Expr* pattern, Expr* condition, Expr* ret_expr);
Expr* create_create_expr(Expr* pattern);
Expr* create_delete_expr(Expr* pattern);
Expr* create_set_expr(Expr* target, Expr* value);
void print_expr(Expr* expr);
struct Expr *get_root();
Expr* create_statement_list(struct Expr* statement, struct Expr* next);
Expr* create_delete_statement(struct Expr* pattern);
Expr* create_create_statement(struct Expr* pattern);
Expr* create_match_statement(struct Expr* pattern, struct Expr* condition, struct Expr* ret_expr);
void free_expr(Expr* expr);

#endif // AST_H
