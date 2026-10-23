
#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUMBER,
    AST_VARIABLE,
    AST_BINARY,
    AST_UNARY,
    AST_FUNCTION,
    AST_EQUATION
} ASTType;

typedef struct ASTNode {
    ASTType type;
    double number;
    char *text;
    char op;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

/* Functions for creating AST nodes */
ASTNode *ast_number(double value);
ASTNode *ast_variable(const char *name);
ASTNode *ast_binary(char op, ASTNode *left, ASTNode *right);
ASTNode *ast_unary(char op, ASTNode *child);
ASTNode *ast_function(const char *name, ASTNode *arg1, ASTNode *arg2);
ASTNode *ast_equation(ASTNode *left, ASTNode *right);

/* Functions for displaying and freeing the AST */
void ast_print(const ASTNode *node);
void ast_print_tree(const ASTNode *node, const char *prefix, int is_last);
void ast_free(ASTNode *node);

#endif
