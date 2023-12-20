/***********AUXILARY ROUTINES******************/
%{
#include <stdio.h>

void yyerror (const char *str) { //ERROR FUNCTION
	fprintf(stderr, "error: %s\n", str);
}

int yywrap() { //WRAP UP IF THE GIVEN INPUT IS COMPLETED
	return 1;
}

main() {
	yyparse(); //EXECUTION BASED ON RULES
}

%}

/********************CHEKCING RULES FOR OUR PARSER************************/
%%

%token SELECT FROM IDENTIFIER WHERE AND;

line: select items using condition '\n' { printf("Syntax Correct\n"); };

select: SELECT;

items: '*' | identifiers;

identifiers: IDENTIFIER | IDENTIFIER ',' identifiers;

using: FROM IDENTIFIER WHERE;

condition: IDENTIFIER '=' IDENTIFIER | IDENTIFIER '=' IDENTIFIER AND condition;

%%
