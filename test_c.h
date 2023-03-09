#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> 



extern int yylineno;
extern char *yytext;
void yyerror(char *msg);
typedef struct Node
{
    int line;
    char *name;
    int tag;
    struct Node *cld[10];
    int ncld;
    char *content;
    char *type;
    float value;
} * SyntaxTreeNode;

SyntaxTreeNode buildTree(char *name, int num, ...);

int hasFault;
int lNum;
int tNum;
char* curType;
int inStruc;
int LCnum;
int strucNum;


typedef struct _OperandStru 
{
    char* type;
    union {
        int tempvar; 
        int lable;   
        int value;   
        char *name;  
    } operand;
    int value;
} OperandStru, *Operand;
typedef struct _InterCodeStru 
{
    char* operation;
    union {
        struct
        {
            Operand left, right;
        } assign;
        struct
        {
            Operand result, op1, op2;
        } binop;
        struct
        { 
            Operand lable, op1, op2;
            char *relop;
        } jump;
        Operand var;
    } operands;
    struct _InterCodeStru *prev, *next;
} InterCodeStru, *Quadruple;

typedef struct _ArgListStru
{
    int num;
    Operand list[10];
} ArgListStru, *ArgList;
char* variable[100];
int charNum;
int findVar(char* name);
int isVar;
Quadruple CodesHead, CodesTail; 

Operand temp_Operands[100];
Operand new_tempvar();
Operand new_lable();
void init_tempvar_lable();
Operand get_Operand(SyntaxTreeNode Exp);
Operand find_Const(int value);
void init_InterCode();
Operand new_Operand();
Operand new_Variable(char *name);
Operand new_Const(float value);
Quadruple new_Code();
Quadruple new_lable_Code(Operand lable);
Quadruple new_goto_Code(Operand lable);
Quadruple new_assign_Code(Operand left, Operand right);
void print_Code(Quadruple code);
void print_Operand(Operand op);
void print_Codes(Quadruple codes);
Quadruple get_Tail(Quadruple codes);
Quadruple add_Codes(int num, ...);
Quadruple translate_Program(SyntaxTreeNode Program);
Quadruple translate_ExtDefList(SyntaxTreeNode ExtDefList);
Quadruple translate_ExtDef(SyntaxTreeNode ExtDef);
Quadruple translate_FunDec(SyntaxTreeNode FunDec);
Quadruple translate_VarList(SyntaxTreeNode VarList);
Quadruple translate_ParamDec(SyntaxTreeNode ParamDec);
Quadruple translate_CompSt(SyntaxTreeNode ComSt);
Quadruple translate_StmtList(SyntaxTreeNode);
Quadruple translate_Stmt(SyntaxTreeNode Stmt);
Quadruple translate_DefList(SyntaxTreeNode DefList);
Quadruple translate_Def(SyntaxTreeNode Def);
Quadruple translate_DecList(SyntaxTreeNode DecList);
Quadruple translate_Dec(SyntaxTreeNode Dec);
Quadruple translate_Exp(SyntaxTreeNode Exp, Operand place);
Quadruple translate_Cond(SyntaxTreeNode Exp, Operand lable_true, Operand lable_false);
Quadruple translate_Args(SyntaxTreeNode Args, ArgList arg_list);


