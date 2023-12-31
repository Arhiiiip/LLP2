// ast.c
#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


Expr* create_int_expr(int value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = INT_EXPR;
    expr->value.int_val = value;
    return expr;
}

Expr* create_float_expr(float value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = FLOAT_EXPR;
    expr->value.float_val = value;
    return expr;
}

Expr* create_string_expr(const char* value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = STRING_EXPR;
    expr->value.str_val = strdup(value);
    return expr;
}

Expr* create_bool_expr(int value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = BOOL_EXPR;
    expr->value.boolean = value;
    return expr;
}

Expr* create_name_expr(const char* value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = NAME_EXPR;
    expr->value.str_val = strdup(value);
    return expr;
}

Expr* create_relation_expr(Expr* left, Expr* right, enum RelationType rel_type) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = RELATION_EXPR;
    expr->value.relation_expr.left = left;
    expr->value.relation_expr.right = right;
    expr->value.relation_expr.rel_type = rel_type;
    return expr;
}

Expr* create_property_expr(Expr* object, Expr* property) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = PROPERTY_EXPR;
    expr->value.property_expr.object = object;
    expr->value.property_expr.property = property;
    return expr;
}

Expr* create_match_expr(Expr* pattern, Expr* condition, Expr* ret_expr) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = MATCH_EXPR;
    expr->value.match_expr.pattern = pattern;
    expr->value.match_expr.condition = condition;
    expr->value.match_expr.ret_expr = ret_expr;
    return expr;
}

Expr* create_create_expr(Expr* pattern) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = CREATE_EXPR;
    expr->value.create_expr.pattern = pattern;
    return expr;
}

Expr* create_delete_expr(Expr* pattern) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = DELETE_EXPR;
    expr->value.delete_expr.pattern = pattern;
    return expr;
}

Expr* create_set_expr(Expr* target, Expr* value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = SET_EXPR;
    expr->value.set_expr.target = target;
    expr->value.set_expr.value = value;
    return expr;
}


struct Expr *root = NULL;


struct Expr *get_root() {
    return root;
}

Expr* create_expr_list(Expr* expr, Expr* next) {
    Expr* expr_list = malloc(sizeof(Expr));
    expr_list->type = LIST_EXPR;
    expr_list->value.expr_list.expr = expr;
    expr_list->value.expr_list.next = next;
    return expr_list;
}

struct Expr* create_match_statement(struct Expr* pattern, struct Expr* condition, struct Expr* ret_expr) {
    struct Expr* match_expr = malloc(sizeof(struct Expr));
    match_expr->type = MATCH_EXPR;
    match_expr->value.match_expr.pattern = pattern;
    match_expr->value.match_expr.condition = condition;
    match_expr->value.match_expr.ret_expr = ret_expr;
    return match_expr;
}

struct Expr* create_create_statement(struct Expr* pattern) {
    struct Expr* create_expr = malloc(sizeof(struct Expr));
    create_expr->type = CREATE_EXPR;
    create_expr->value.create_expr.pattern = pattern;
    return create_expr;
}

struct Expr* create_delete_statement(struct Expr* pattern) {
    struct Expr* delete_expr = malloc(sizeof(struct Expr));
    delete_expr->type = DELETE_EXPR;
    delete_expr->value.delete_expr.pattern = pattern;
    return delete_expr;
}

struct Expr* create_statement_list(struct Expr* statement, struct Expr* next) {
    struct Expr* expr_list = malloc(sizeof(struct Expr));
    expr_list->type = STATEMENT_LIST;
    expr_list->value.statement_list.statement = statement;
    expr_list->value.statement_list.next = next;
    return expr_list;
}




void print_expr(Expr* expr) {
    if (!expr) {
        return;
    }

    switch (expr->type) {
        case INT_EXPR:
            printf("INT_EXPR: %d\n", expr->value.int_val);
            break;

        case FLOAT_EXPR:
            printf("FLOAT_EXPR: %f\n", expr->value.float_val);
            break;

        case STRING_EXPR:
            printf("STRING_EXPR: %s\n", expr->value.str_val);
            break;

        case BOOL_EXPR:
            printf("BOOL_EXPR: %s\n", expr->value.boolean ? "true" : "false");
            break;

        case NAME_EXPR:
            printf("NAME_EXPR: %s\n", expr->value.str_val);
            break;

        case RELATION_EXPR:
            printf("RELATION_EXPR:\n");
            printf("Left:\n");
            print_expr(expr->value.relation_expr.left);
            printf("Right:\n");
            print_expr(expr->value.relation_expr.right);
            break;

            // Добавьте обработку других типов, если необходимо

        default:
            printf("Unknown expression type\n");
            break;
    }
}

// Дополните реализации других функций, если необходимо

// Реализация функции для освобождения ресурсов
void free_expr(Expr* expr) {
    if (!expr) {
        return;
    }

    switch (expr->type) {
        case STRING_EXPR:
        case NAME_EXPR:
            free(expr->value.str_val);
            break;

        case RELATION_EXPR:
            free_expr(expr->value.relation_expr.left);
            free_expr(expr->value.relation_expr.right);
            break;

        case PROPERTY_EXPR:
            free_expr(expr->value.property_expr.object);
            free_expr(expr->value.property_expr.property);
            break;


        case LIST_EXPR:
            free_expr(expr->value.expr_list.expr);
            free_expr(expr->value.expr_list.next);
            break;


        case MATCH_EXPR:
            free_expr(expr->value.match_expr.pattern);
            free_expr(expr->value.match_expr.condition);
            free_expr(expr->value.match_expr.ret_expr);
            break;

        case CREATE_EXPR:
            free_expr(expr->value.create_expr.pattern);
            break;

        case DELETE_EXPR:
            free_expr(expr->value.delete_expr.pattern);
            break;

        case SET_EXPR:
            free_expr(expr->value.set_expr.target);
            free_expr(expr->value.set_expr.value);
            break;

            // Добавьте освобождение ресурсов для других типов, если необходимо

        default:
            break;
    }

    free(expr);
}
