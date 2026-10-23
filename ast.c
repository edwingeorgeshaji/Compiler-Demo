
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Create a number node */
ASTNode *ast_number(double value)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = AST_NUMBER;
    node->number = value;
    return node;
}

/* Create a variable node */
ASTNode *ast_variable(const char *name)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = AST_VARIABLE;
    node->text = malloc(strlen(name) + 1);

    if (node->text == NULL) {
        free(node);
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(node->text, name);
    return node;
}

/* Create a binary operator node */
ASTNode *ast_binary(char op, ASTNode *left, ASTNode *right)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = AST_BINARY;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

/* Create a unary operator node */
ASTNode *ast_unary(char op, ASTNode *child)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = AST_UNARY;
    node->op = op;
    node->left = child;
    return node;
}

/* Create a function node, such as sqrt() or pow() */
ASTNode *ast_function(const char *name, ASTNode *arg1, ASTNode *arg2)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = AST_FUNCTION;
    node->text = malloc(strlen(name) + 1);

    if (node->text == NULL) {
        free(node);
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(node->text, name);
    node->left = arg1;
    node->right = arg2;
    return node;
}

/* Create an equation node */
ASTNode *ast_equation(ASTNode *left, ASTNode *right)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = AST_EQUATION;
    node->left = left;
    node->right = right;
    return node;
}

/* Print AST in compact form */
void ast_print(const ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->type) {
        case AST_NUMBER:
            printf("%g", node->number);
            break;

        case AST_VARIABLE:
            printf("%s", node->text);
            break;

        case AST_BINARY:
            printf("(");
            ast_print(node->left);
            printf(" %c ", node->op);
            ast_print(node->right);
            printf(")");
            break;

        case AST_UNARY:
            printf("(%c", node->op);
            ast_print(node->left);
            printf(")");
            break;

        case AST_FUNCTION:
            printf("%s(", node->text);
            ast_print(node->left);

            if (node->right != NULL) {
                printf(", ");
                ast_print(node->right);
            }

            printf(")");
            break;

        case AST_EQUATION:
            ast_print(node->left);
            printf(" = ");
            ast_print(node->right);
            break;
    }
}

/* Get the label displayed for each tree node */
static void print_node_label(const ASTNode *node)
{
    switch (node->type) {
        case AST_NUMBER:
            printf("%g", node->number);
            break;

        case AST_VARIABLE:
            printf("%s", node->text);
            break;

        case AST_BINARY:
            printf("%c", node->op);
            break;

        case AST_UNARY:
            printf("unary %c", node->op);
            break;

        case AST_FUNCTION:
            printf("%s", node->text);
            break;

        case AST_EQUATION:
            printf("=");
            break;
    }
}

/* Print AST as a branching tree */
void ast_print_tree(const ASTNode *node, const char *prefix, int is_last)
{
    char new_prefix[512];

    if (node == NULL)
        return;

    printf("%s", prefix);
    printf("%s", is_last ? "`-- " : "|-- ");
    print_node_label(node);
    printf("\n");

    snprintf(new_prefix, sizeof(new_prefix), "%s%s",
             prefix, is_last ? "    " : "|   ");

    if (node->left != NULL && node->right != NULL) {
        ast_print_tree(node->left, new_prefix, 0);
        ast_print_tree(node->right, new_prefix, 1);
    } else if (node->left != NULL) {
        ast_print_tree(node->left, new_prefix, 1);
    } else if (node->right != NULL) {
        ast_print_tree(node->right, new_prefix, 1);
    }
}

/* Release the memory used by the AST */
void ast_free(ASTNode *node)
{
    if (node == NULL)
        return;

    ast_free(node->left);
    ast_free(node->right);

    free(node->text);
    free(node);
}
