/***************AUXILARY ROUTINES*******************/

%{
#include <stdio.h>
#include "y.tab.h"
extern int yylval; //PASS VALUE ASSOCICATED WITH TOKEN
%}

/********************DEFINING RULES WITH GRAMMER***************************/
%%
select					return SELECT;
from					return FROM;
where					return WHERE;
and						return AND;
[*]						return *yytext;
[,]						return *yytext;
[=]						return *yytext;
[a-zA-Z][a-zA-Z0-9]+	return IDENTIFIER;
\n						return *yytext;
[ \t]+					/* ignore whitespace */;
%%
