%{
#include <ctype.h>
#include <stdio.h>
#define YYSTYPE double // define the semantic stack as double type
#define YYDEBUG 1
#define YYDEBUG_LEXER_TEXT lexeme
int yyerror(const char*);
int yylex(void);
%}
%token NUMBER
%left '+' '-'
%left '*' '/'
%right UMINUS
%%
lines : lines expr '\n' {printf("%lf\n", $2);};
	| lines '\n'
	| /* null str */
	| error '\n' {yyerror("reenter last line:"); }
	;
expr : expr '+' expr {$$ = $1 + $3;}
	| expr '-' expr { $$ = $1 - $3; }
	| expr '*' expr { $$ = $1 * $3; }
	| expr '/' expr { $$ = $1 / $3; }
	| '(' expr ')' { $$ = $2; }
	| '(' expr error { $$ = $2; yyerror("missing ')'"); }
	| '-' expr %prec UMINUS { $$ = -$2; }
	| NUMBER
	;
%%

int yylex(void) {
	int c;
	while ((c = getchar()) == ' ');
	if (c == '.' || isdigit(c)) {
		ungetc(c, stdin);
		scanf("%lf", &yylval);
		return NUMBER;
	}
	return c;
}
