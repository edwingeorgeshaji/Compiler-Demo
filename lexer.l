
%option noyywrap

%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>

static char *copy_lexeme(const char *text)
{
    size_t length = strlen(text) + 1;
    char *copy = malloc(length);

    if (copy == NULL)
        exit(EXIT_FAILURE);

    memcpy(copy, text, length);
    return copy;
}
%}

%%

([0-9]+(\.[0-9]+)?|\.[0-9]+) {
    yylval.number = atof(yytext);
    return NUMBER;
}

"sqrt"                  { return SQRT; }
"pow"                   { return POW; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.text = copy_lexeme(yytext);
    return IDENTIFIER;
}

"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"^"                     { return '^'; }
"="                     { return '='; }

"("                     { return '('; }
")"                     { return ')'; }
","                     { return ','; }

[ \t\r]+                { /* Ignore whitespace */ }
\n                      { return '\n'; }

.                       { return INVALID; }

%%
