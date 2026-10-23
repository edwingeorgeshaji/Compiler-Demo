
%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "semantic.h"

int yylex(void);
void yyerror(const char *message);

ASTNode *root = NULL;
%}

%define parse.error verbose

%union {
    double number;
    char *text;
    ASTNode *node;
}

%token <number> NUMBER
%token <text> IDENTIFIER
%token SQRT POW INVALID

%type <node> expression

%left '+' '-'
%left '*' '/'
%right UMINUS
%right '^'

%%

input:
      expression '\n'
      {
          root = $1;
          YYACCEPT;
      }
    | expression '=' expression '\n'
      {
          root = ast_equation($1, $3);
          YYACCEPT;
      }
    ;

expression:
      expression '+' expression
      {
          $$ = ast_binary('+', $1, $3);
      }
    | expression '-' expression
      {
          $$ = ast_binary('-', $1, $3);
      }
    | expression '*' expression
      {
          $$ = ast_binary('*', $1, $3);
      }
    | expression '/' expression
      {
          $$ = ast_binary('/', $1, $3);
      }
    | expression '^' expression
      {
          $$ = ast_binary('^', $1, $3);
      }
    | '-' expression %prec UMINUS
      {
          $$ = ast_unary('-', $2);
      }
    | NUMBER
      {
          $$ = ast_number($1);
      }
    | IDENTIFIER
      {
          $$ = ast_variable($1);
          free($1);
      }
    | '(' expression ')'
      {
          $$ = $2;
      }
    | SQRT '(' expression ')'
      {
          $$ = ast_function("sqrt", $3, NULL);
      }
    | POW '(' expression ',' expression ')'
      {
          $$ = ast_function("pow", $3, $5);
      }
    ;

%%

void yyerror(const char *message)
{
    fprintf(stderr, "Invalid formula: %s\n", message);
}

int main(void)
{
    int result;

    printf("Enter a mathematical expression or equation:\n");
    fflush(stdout);

    result = yyparse();

    if (result != 0 || root == NULL) {
        ast_free(root);
        return EXIT_FAILURE;
    }

    if (!semantic_check(root)) {
        ast_free(root);
        return EXIT_FAILURE;
    }

    printf("Valid mathematical formula!\n");
    printf("AST Tree:\n");
    ast_print_tree(root, "", 1);

    ast_free(root);
    return EXIT_SUCCESS;
}
