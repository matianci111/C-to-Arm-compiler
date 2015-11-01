%{
	#include <iostream>
	#include <string>
	#include "y.tab.h"
	void yyerror(char *);

%}


DIGIT    [0-9]
ID       [a-zA-Z][a-zA-Z0-9]*
L		 [a-zA-Z_]


%%
"#include <"{ID}"."{ID}">"	{yylval.id = yytext; return P_INC;}
\"(\\.|[^\\"])*\"	{yylval.id = strdup(yytext);return STRING_LITERAL;}
"double"			return DOUBLE;
"int"				return INT;
"for"				{return FOR;}
"void"				return VOID;
"const"				return CONST;
"char"				return CHAR;
"return"			return RETURN;
"if"				return IF;
"else"				return ELSE;
"while"				return WHILE;
"..."				{return ARG_LIST;}
{L}({L}|{DIGIT})*               {yylval.id = strdup(yytext);return IDENTIFIER;}
{DIGIT}+"."{DIGIT}+ {yylval.dvalue = atof(yytext); return dCONSTANT;}
{DIGIT}+			{yylval.cvalue = atoi(yytext); return iCONSTANT;}
"++" 			return INCREMENT;
"--"			return DECREMENT;
"+"				return ('+');
"-"				return ('-');
"*"				return ('*');
"/"				return ('/');
"="				return ('=');
"=="			return EQ;
">"				return ('>');
">="			return GE;
"<"				return ('<');
"<="			return LE;
"!="			return NE;
"("				return ('(');
")"				return (')');
"{"				return ('{');
"}"				return ('}');
";"				return (';');
","				return (',');
[ \t\n]+        		; 
.				yyerror("unknown");

%%
int yywrap(void){
	return 1;
}

