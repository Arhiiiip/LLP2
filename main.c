#include "parser/ast.h"
#include "parser/parser.tab.h"

int main() {
    yyparse();
    print_expr(get_root());
    free_expr(get_root());
    return 0;
}
