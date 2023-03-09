%{
#include<unistd.h>
#include<stdio.h>   
#include "test_c.h"
int yylex();
%}

%union{
    SyntaxTreeNode type;
    double d;
}

%token <type> INT FLOAT CHAR
%token <type> ID STRUCT RETURN IF ELSE WHILE COMMENT SPACE SEMI COMMA ASSIGN PLUS INT_ FLOAT_ CHAR_
%token <type> MINUS MUL DIV AND OR DOT NOT LP RP LB RB LC RC AERROR LT LE GT GE NE EQ EOL

%type  <type> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier 
%type  <type> VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args


%right ASSIGN
%left OR
%left AND
%left LT LE GT GE NE EQ
%left PLUS MINUS
%left MUL DIV
%right NOT 
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE 
%nonassoc ELSE


%%

Program:ExtDefList {$$=buildTree("Program",1,$1);Quadruple codes = translate_Program($$);print_Codes(codes);}
    ;
ExtDefList:ExtDef ExtDefList {$$=buildTree("ExtDefList",2,$1,$2);}
        | {$$=buildTree("ExtDefList",0,-1);}
        ;
ExtDef:Specifier ExtDecList SEMI    {$$=buildTree("ExtDef",3,$1,$2,$3); }    
        |Specifier SEMI {$$=buildTree("ExtDef",2,$1,$2);}
        |Specifier FunDec CompSt    {$$=buildTree("ExtDef",3,$1,$2,$3);}
        |Specifier ExtDecList error {yyerror("Missing semicolon ';'");}
        |Specifier error {yyerror("Missing semicolon ';'");}
        ;
ExtDecList:VarDec {$$=buildTree("ExtDecList",1,$1);}
        |VarDec COMMA ExtDecList {$$=buildTree("ExtDecList",3,$1,$2,$3);}
        ;

Specifier:INT_ {$$=buildTree("Specifier",1,$1);}
        | FLOAT_ {$$=buildTree("Specifier",1,$1);}
        | CHAR_ {$$=buildTree("Specifier",1,$1);}
        |StructSpecifier {$$=buildTree("Specifier",1,$1);}
        ;
StructSpecifier:STRUCT ID LC DefList RC {$$=buildTree("StructSpecifier",5,$1,$2,$3,$4,$5);}
        |STRUCT ID {$$=buildTree("StructSpecifier",2,$1,$2);}
        ;

VarDec:ID {$$=buildTree("VarDec",1,$1);}
        |VarDec LB INT RB {$$=buildTree("VarDec",4,$1,$2,$3,$4);}
        ;
FunDec:ID LP VarList RP {$$=buildTree("FunDec",4,$1,$2,$3,$4); }
        |ID LP RP {$$=buildTree("FunDec",3,$1,$2,$3);}
        |ID LP VarList error {yyerror("Missing closing parenthesis ')'");}
        |ID LP error {yyerror("Missing closing parenthesis ')'");}
        ;
VarList:ParamDec COMMA VarList {$$=buildTree("VarList",3,$1,$2,$3);}
        |ParamDec {$$=buildTree("VarList",1,$1);}
        ;
ParamDec:Specifier VarDec {$$=buildTree("ParamDec",2,$1,$2);  }
    ;


CompSt:LC DefList StmtList RC {$$=buildTree("CompSt",4,$1,$2,$3,$4);}
        ;
StmtList:Stmt StmtList{$$=buildTree("StmtList",2,$1,$2);}
        | {$$=buildTree("StmtList",0,-1);}
        ;
Stmt:Exp SEMI {$$=buildTree("Stmt",2,$1,$2);}
        |CompSt {$$=buildTree("Stmt",1,$1);}
        |RETURN Exp SEMI {$$=buildTree("Stmt",3,$1,$2,$3);}
        |IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=buildTree("Stmt",5,$1,$2,$3,$4,$5);}
        |IF LP Exp RP Stmt ELSE Stmt {$$=buildTree("Stmt",7,$1,$2,$3,$4,$5,$6,$7);}
        |WHILE LP Exp RP Stmt {$$=buildTree("Stmt",5,$1,$2,$3,$4,$5);}
        |RETURN Exp error {yyerror("Missing semicolon ';'");}
        |Exp error {yyerror("Missing semicolon ';'");}
        ;

DefList:Def DefList{$$=buildTree("DefList",2,$1,$2);}
        | {$$=buildTree("DefList",0,-1);}
        ;
Def:Specifier DecList SEMI {$$=buildTree("Def",3,$1,$2,$3);}
        ;
DecList:Dec {$$=buildTree("DecList",1,$1);}
        |Dec COMMA DecList {$$=buildTree("DecList",3,$1,$2,$3);}
        ;
Dec:VarDec {$$=buildTree("Dec",1,$1);}
        |VarDec ASSIGN Exp {$$=buildTree("Dec",3,$1,$2,$3);}
        ;

Exp:Exp ASSIGN Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp AND Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp OR Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp LT Exp {$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp LE Exp {$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp GT Exp {$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp GE Exp {$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp NE Exp {$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp EQ Exp {$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp PLUS Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp MINUS Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp MUL Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |Exp DIV Exp{$$=buildTree("Exp",3,$1,$2,$3);}
        |LP Exp RP{$$=buildTree("Exp",3,$1,$2,$3);}
        |LP Exp error{yyerror("Missing closing parenthesis ')'");}
        |MINUS Exp {$$=buildTree("Exp",2,$1,$2);}
        |NOT Exp {$$=buildTree("Exp",2,$1,$2);}
        |ID LP Args RP {$$=buildTree("Exp",4,$1,$2,$3,$4);}
        |ID LP RP {$$=buildTree("Exp",3,$1,$2,$3);}
        |ID LP Args error {yyerror("Missing closing parenthesis ')'");}
        |ID LP error {yyerror("Missing closing parenthesis ')'");}
        |Exp LB Exp RB {$$=buildTree("Exp",4,$1,$2,$3,$4);}
        |Exp DOT ID {$$=buildTree("Exp",3,$1,$2,$3);}
        |ID {$$=buildTree("Exp",1,$1);}
        |INT {$$=buildTree("Exp",1,$1);}
        |FLOAT{$$=buildTree("Exp",1,$1);}
        |CHAR{$$=buildTree("EXP",1,$1);}
        ;
Args:Exp COMMA Args {$$=buildTree("Args",3,$1,$2,$3);}
        |Exp {$$=buildTree("Args",1,$1);}
        ;
%%
