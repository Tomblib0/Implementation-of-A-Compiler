%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "test_c.h"
    #include "syntax.tab.h"
%}

%option yylineno

INT (0|[1-9][0-9]*)|(0[xX][a-fA-F0-9]+)

FLOAT (0|[1-9][0-9]*)\.[0-9]+
CHAR ['][a-zA-Z][']|['][\\][xX][1-9a-fA-F][0-9a-fA-F][']

ID [a-z_A-Z][a-z_A-Z0-9]*

STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
INT_ int
FLOAT_ float
CHAR_ char
SEMI  ;
COMMA ,
ASSIGN  =
PLUS  \+
MINUS \-
MUL  \*
DIV   \/
AND   &&
OR    \|\|
DOT   \.
NOT   \!
LP    \(
RP    \)
LB    \[
RB    \]
LC    \{
RC    \}
LT  <
LE  <=
GT  >
GE  >=
NE  !=
EQ  ==


COMMENT ("//".*)|("/*"([*]*(([^*/])+([/])*)*)*"*/")

SPACE [ \f\r\t\v]+
EOL \n
error0 .
error1 [0][0-9]+
error2 [0-9]+[a-w_A-Wy-zY-Z][a-z_A-Z0-9]*
error3 0[xX]0[a-fA-F0-9]+
error4 0[xX][0-9a-zA-Z]*[g-zG-Z][0-9a-zA-Z]*
error5 ['][\\][xX][0-9a-zA-Z]+[']
%%

{SPACE} {}
{COMMENT} {}
{EOL} {}

{INT_} {yylval.type=buildTree("int",0,yylineno);return INT_;}
{FLOAT_} {yylval.type=buildTree("float",0,yylineno);return FLOAT_;}
{CHAR_} {yylval.type=buildTree("char",0,yylineno);return CHAR_;}
{STRUCT} {yylval.type=buildTree("STRUCT",0,yylineno);return STRUCT;}
{RETURN} {yylval.type=buildTree("RETURN",0,yylineno); return RETURN;}
{IF} {yylval.type=buildTree("IF",0,yylineno);return IF;}
{ELSE} {yylval.type=buildTree("ELSE",0,yylineno); return ELSE;}
{WHILE} {yylval.type=buildTree("WHILE",0,yylineno); return WHILE;}

{error1} {printf("Error type A at line %d: unknown lexeme %s\n",yylineno,yytext);}
{error2} {printf("Error type A at line %d: unknown lexeme %s\n",yylineno,yytext);}
{error3} {printf("Error type A at line %d: unknown lexeme %s\n",yylineno,yytext);}
{error4} {printf("Error type A at line %d: unknown lexeme %s\n",yylineno,yytext);}

{INT} {yylval.type=buildTree("INT",0,yylineno); return INT;}
{FLOAT} {yylval.type=buildTree("FLOAT",0,yylineno); return FLOAT;}
{CHAR} {yylval.type=buildTree("CHAR",0,yylineno); return CHAR;}
{error5} {printf("Error type A at line %d: unknown lexeme %s\n",yylineno,yytext);}


{SEMI} {yylval.type=buildTree("SEMI",0,yylineno); return SEMI;}
{COMMA} {yylval.type=buildTree("COMMA",0,yylineno); return COMMA;}
{ASSIGN} {yylval.type=buildTree("ASSIGN",0,yylineno); return ASSIGN;}
{PLUS} {yylval.type=buildTree("PLUS",0,yylineno); return PLUS;}
{MINUS} {yylval.type=buildTree("MINUS",0,yylineno); return MINUS;}
{MUL} {yylval.type=buildTree("MUL",0,yylineno); return MUL;}
{DIV} {yylval.type=buildTree("DIV",0,yylineno); return DIV;}
{AND} {yylval.type=buildTree("AND",0,yylineno); return AND;}
{OR} {yylval.type=buildTree("OR",0,yylineno); return OR;}
{DOT} {yylval.type=buildTree("DOT",0,yylineno); return DOT;}
{NOT} {yylval.type=buildTree("NOT",0,yylineno); return NOT;}
{LP} {yylval.type=buildTree("LP",0,yylineno); return LP;}
{RP} {yylval.type=buildTree("RP",0,yylineno); return RP;}
{LB} {yylval.type=buildTree("LB",0,yylineno); return LB;}
{RB} {yylval.type=buildTree("RB",0,yylineno); return RB;}
{LC} {yylval.type=buildTree("LC",0,yylineno); return LC;}
{RC} {yylval.type=buildTree("RC",0,yylineno); return RC;}
{LT} {yylval.type=buildTree("LT",0,yylineno);return LT;}
{LE} {yylval.type=buildTree("LE",0,yylineno);return LE;}
{GT} {yylval.type=buildTree("GT",0,yylineno);return GT;}
{GE} {yylval.type=buildTree("GE",0,yylineno);return GE;}
{NE} {yylval.type=buildTree("NE",0,yylineno);return NE;}
{EQ} {yylval.type=buildTree("EQ",0,yylineno);return EQ;}

{ID} {yylval.type=buildTree("ID",0,yylineno); return ID;}

{error0} {
    printf("Error type A at line %d: unknown lexeme %s\n",yylineno,yytext);
}
%%


int yywrap()
{
    return 1;
}
