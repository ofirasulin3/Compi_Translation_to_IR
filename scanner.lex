%{
/* Declarations section */
#include "helpers.h"
#define YYSTYPE Node*
#include "parser.tab.hpp"
#include "hw3_output.hpp"
#include <stdio.h>
using namespace output;

%}

%option yylineno
%option noyywrap
digit           ([0-9])
positive		([1-9])
letter          ([a-zA-Z])
char            ([a-zA-Z0-9])
ignore 			[\n\r\t\x0a\x0d ]+

%%
void								return VOID;
int									return INT;
byte	   							return BYTE;
b									return B;
bool								return BOOL;
and									return AND;
or									return OR;
not									return NOT;
true								return TRUE;
false								return FALSE;
return								return RETURN;
if									return IF;
else								return ELSE;
while								return WHILE;
break								return BREAK;
continue							return CONTINUE;
switch								return SWITCH;
case								return CASE;
default							    return DEFAULT;
\:									return COLON;
\;     			        		    return SC;
\,     	        				    return COMMA;
\(									return LPAREN;
\)									return RPAREN;
\{									return LBRACE;
\}									return RBRACE;
\=									return ASSIGN;
\<|\>|\>=|\<=						return RELOPREL;
==|!=							    return RELOPEQ;
\+|-						        return BINOPPLUS;
\*|\/					       		return BINOPMUL;
[a-zA-Z][a-zA-Z0-9]*				{yylval = new Node(yytext); return ID;}
0|[1-9][0-9]* 						{yylval = new Node(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"   	{yylval = new Node(yytext); return STRING;}
\/\/[^\r\n]*(\r|\n|\r\n)?			{};
{ignore}							{};
.               					{output::errorLex(yylineno); exit(0);};
%%


