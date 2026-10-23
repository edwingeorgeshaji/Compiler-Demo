#include <stdio.h>
#include <math.h>
#include <string.h>
#include "semantic.h"

/* Calculate a value only if the entire expression is constant.
   Expressions containing variables are not evaluated. */
static int constant_value(const ASTNode *node, double *value)
{
    double a, b;

    if (node == NULL)
        return 0;

    switch (node->type) {
        case AST_NUMBER:
            *value = node->number;
            return 1;

        case AST_UNARY:
            if (node->op == '-' &&
                constant_value(node->left, &a)) {
                *value = -a;
                return 1;
            }
            return 0;

        case AST_BINARY:
            if (!constant_value(node->left, &a) ||
                !constant_value(node->right, &b))
                return 0;

            switch (node->op) {
                case '+': *value = a + b; return 1;
                case '-': *value = a - b; return 1;
                case '*': *value = a * b; return 1;

                case '/':
                    if (fabs(b) < 1e-12)
                        return 0;
                    *value = a / b;
                    return 1;

                case '^':
                    *value = pow(a, b);
                    return isfinite(*value);
            }
            return 0;

        case AST_FUNCTION:
            if (strcmp(node->text, "sqrt") == 0) {
                if (!constant_value(node->left, &a) || a < 0)
                    return 0;

                *value = sqrt(a);
                return isfinite(*value);
            }

            if (strcmp(node->text, "pow") == 0) {
                if (!constant_value(node->left, &a) ||
                    !constant_value(node->right, &b))
                    return 0;

                *value = pow(a, b);
                return isfinite(*value);
            }
            return 0;

        case AST_VARIABLE:
        case AST_EQUATION:
            return 0;
    }

    return 0;
}

int semantic_check(const ASTNode *node)
{
    double value;

    if (node == NULL)
        return 0;

    /* Check both children first. */
    if (node->left != NULL &&
        !semantic_check(node->left))
        return 0;

    if (node->right != NULL &&
        !semantic_check(node->right))
        return 0;

    /* Reject division by a constant expression equal to zero. */
    if (node->type == AST_BINARY && node->op == '/') {
        if (constant_value(node->right, &value) &&
            fabs(value) < 1e-12) {
            fprintf(stderr,
                    "Semantic error: division by zero.\n");
            return 0;
        }
    }

    /* Reject sqrt() when its argument is a known negative value. */
    if (node->type == AST_FUNCTION &&
        strcmp(node->text, "sqrt") == 0) {
        if (constant_value(node->left, &value) &&
            value < 0) {
            fprintf(stderr,
                    "Semantic error: sqrt of a negative value.\n");
            return 0;
        }
    }

    return 1;
}
