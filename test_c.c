#include "test_c.h"
int i;
SyntaxTreeNode buildTree(char *name, int num, ...)
{
    SyntaxTreeNode father = (SyntaxTreeNode)malloc(sizeof(struct Node));
    father->name = name;
    father->ncld = num;
    father->type = NULL;
    va_list ap;
    va_start(ap, num);
     for(int j=0;j<num;j++){
       (father->cld)[j] = va_arg(ap, SyntaxTreeNode);
       if(j==0){
         father->line = (father->cld)[0] ->line;
         father->type = (father->cld)[0] ->type;
         father->content = (father->cld)[0] ->content;
         father->tag = (father->cld)[0] ->tag;
       }
     }
     if(num==0){
        if(strcmp(name,"INT")==0){
           father->line=va_arg(ap,int);
           father->value = atoi(yytext);
           father->type = "int";
        }else if(strcmp(name,"FLOAT")==0){
           father->line=va_arg(ap,int);
           father->value = atof(yytext);
           father->type = "float";
        }else{
           father->line=va_arg(ap,int);
           father->content=(char *)malloc(sizeof(char) * 30);
           strcpy(father->content, yytext);
        }
     }
    va_end(ap);
    return father;
}

void yyerror(char *msg)
{
    hasFault = 1;
    fprintf(stderr, "Error type B at Line %d: %s before %s\n", yylineno, msg, yytext);
}


int main(int argc, char **argv)
{
    int j, tem;
    if (argc < 2)
    {
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        inStruc = 0;
        LCnum = 0;
        strucNum = 0;
        lNum=1;
        tNum=1;
        charNum=0;
        isVar=1;
        FILE *f = fopen(argv[i], "r");
        if (!f)
        {
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yyparse();
        fclose(f);
    }
}

Operand new_tempvar()
{
    Operand result = new_Operand();
    result->type="TEMPVAR";
    result->operand.tempvar = tNum;
    temp_Operands[tNum-1] = result;
    tNum=tNum+1;
    return result;
}
Operand new_lable()
{
    Operand result = new_Operand();
    result->type="LABLE";
    result->operand.lable = lNum;
    lNum=lNum+1;
    return result;
}

Operand new_Operand()
{
    Operand result = (Operand)malloc(sizeof(OperandStru));
    result->value = -10000;
    return result;
}

Operand new_Variable(char *name)
{
    Operand result = new_Operand();
    result->type="VARIABLE";
    result->operand.name = name;
    return result;
}

Operand new_Const(float value)
{
    Operand result = new_Operand();
    result->type="CONSTANT";
    result->operand.value = (int)value;
    result->value = (int)value;
    return result;
}

Quadruple new_Code()
{
    Quadruple result = (Quadruple)malloc(sizeof(InterCodeStru));
    result->operation="NULL";
    result->prev = NULL;
    result->next = NULL;
    return result;
}

Quadruple new_lable_Code(Operand lable)
{
    Quadruple result = new_Code();
    result->operation="LABLE";
    result->operands.var = lable;
    return result;
}

Quadruple new_goto_Code(Operand lable)
{
    Quadruple result = new_Code();
    result->operation="GOTO";
    result->operands.jump.lable = lable;
    return result;
}

Quadruple new_assign_Code(Operand left, Operand right)
{
    left->value = right->value;
    Quadruple result = new_Code();
    result->operation="ASSIGN";
    result->operands.assign.left = left;
    result->operands.assign.right = right;
    return result;
}

void print_Code(Quadruple code)
{
    if (code == NULL)
    {
        printf("InterCode is NULL\n");
        return;
    }
    if(strcmp(code->operation,"LABLE")==0){
        printf("LABLE ");
        print_Operand(code->operands.var);
        printf(" :\n");
    }else if(strcmp(code->operation,"FUNCTION")==0){
        printf("FUNCTION ");
        isVar=0;
        print_Operand(code->operands.var);
        isVar=1;
        printf(" :\n");
    }else if(strcmp(code->operation,"ASSIGN")==0){
        print_Operand(code->operands.assign.left);
        printf(" := ");
        print_Operand(code->operands.assign.right);
        printf("\n");
    }else if(strcmp(code->operation,"ADD")==0){
        print_Operand(code->operands.binop.result);
        printf(" := ");
        print_Operand(code->operands.binop.op1);
        printf(" + ");
        print_Operand(code->operands.binop.op2);
        printf("\n");
    }else if(strcmp(code->operation,"SUB")==0){
        print_Operand(code->operands.binop.result);
        printf(" := ");
        print_Operand(code->operands.binop.op1);
        printf(" - ");
        print_Operand(code->operands.binop.op2);
        printf("\n");
    }else if(strcmp(code->operation,"MUL")==0){
        print_Operand(code->operands.binop.result);
        printf(" := ");
        print_Operand(code->operands.binop.op1);
        printf(" * ");
        print_Operand(code->operands.binop.op2);
        printf("\n");
    }else if(strcmp(code->operation,"DIV")==0){
        print_Operand(code->operands.binop.result);
        printf(" := ");
        print_Operand(code->operands.binop.op1);
        printf(" / ");
        print_Operand(code->operands.binop.op2);
        printf("\n");
    }else if(strcmp(code->operation,"GOTO")==0){
        printf("GOTO ");
        print_Operand(code->operands.jump.lable);
        printf("\n");
    }else if(strcmp(code->operation,"IFGOTO")==0){
        printf("IF ");
        print_Operand(code->operands.jump.op1);
        printf(" %s ", code->operands.jump.relop);
        print_Operand(code->operands.jump.op2);
        printf(" GOTO ");
        print_Operand(code->operands.jump.lable);
        printf("\n");
    }else if(strcmp(code->operation,"RETURN")==0){
        printf("RETURN ");
        print_Operand(code->operands.var);
        printf("\n");
    }else if(strcmp(code->operation,"PARAM")==0){
        printf("PARAM ");
        print_Operand(code->operands.var);
        printf("\n");
    }else if(strcmp(code->operation,"ARG")==0){
        printf("ARG ");
        print_Operand(code->operands.var);
        printf("\n");
    }else if(strcmp(code->operation,"CALL")==0){
        if (code->operands.assign.left == NULL)
        {
            printf("CALL ");
        }
        else
        {
            print_Operand(code->operands.assign.left);
            printf(" := CALL ");
        }
        isVar=0;
        print_Operand(code->operands.assign.right);
        isVar=1;
        printf("\n");
    }else if(strcmp(code->operation,"READ")==0){
        printf("READ ");
        print_Operand(code->operands.var);
        printf("\n");
    }else if(strcmp(code->operation,"WRITE")==0){
        printf("WRITE ");
        print_Operand(code->operands.var);
        printf("\n");
    }
}
int findVar(char* name){
   int flag=0;
   int retVal=-1;
   for(int i=0;i<charNum;i++){
      if(strcmp(name,variable[i])==0){
         retVal=i+1;
         flag=1;
         break;
      }
   }
   if(flag==0){
     variable[charNum]=name;
     charNum++;
     retVal=charNum;
   }
   return retVal;
}

void print_Operand(Operand op)
{
    if (op == NULL)
    {
        printf("Operand is NULL\n");
        return;
    }
    if(strcmp(op->type,"VARIABLE")==0){
       if(isVar){
         int num=findVar(op->operand.name);
         printf("v%d", num);
       }else{
         printf("%s", op->operand.name);
       }
    }else if(strcmp(op->type,"FUNC")==0){
       printf("%s", op->operand.name);
    }else if(strcmp(op->type,"TEMPVAR")==0){
       printf("t%d", op->operand.tempvar);
    }else if(strcmp(op->type,"LABLE")==0){
       printf("lable%d", op->operand.lable);
    }else if(strcmp(op->type,"CONSTANT")==0){
       printf("#%d", op->operand.value);
    }else if(strcmp(op->type,"ADDRESS")==0){
       printf("&%s", op->operand.name);
    }else if(strcmp(op->type,"VALUE")==0){
       printf("#%s", op->operand.name);
    }else{
       printf("error");
    }
}

void print_Codes(Quadruple codes)
{
    Quadruple temp = codes;
    while (temp)
    {
        print_Code(temp);
        temp = temp->next;
    }
}

Quadruple get_Tail(Quadruple codes)
{
    Quadruple temp = codes;
    while (temp->next)
    {
        temp = temp->next;
    }
    return temp;
}

Quadruple add_Codes(int num, ...)
{
    int i;
    va_list ap;
    va_start(ap, num);
    Quadruple code = va_arg(ap, Quadruple);
    Quadruple temp = new_Code();
    Quadruple tail = new_Code();
    for (i = 1; i < num; i++)
    {
        temp = va_arg(ap, Quadruple);
        tail = get_Tail(code);
        tail->next = temp;  
        temp->prev = tail;
    }
    va_end(ap);
    return code;
}

Quadruple translate_Program(SyntaxTreeNode Program)
{
    if (Program->ncld == 1)
    {
        return translate_ExtDefList((Program->cld)[0]);
    }
    return new_Code();
}
Quadruple translate_ExtDefList(SyntaxTreeNode ExtDefList)
{
    if (ExtDefList->ncld == 2)
    {
        Quadruple code1 = translate_ExtDef((ExtDefList->cld)[0]);
        Quadruple code2 = translate_ExtDefList((ExtDefList->cld)[1]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
Quadruple translate_ExtDef(SyntaxTreeNode ExtDef)
{
   if (ExtDef->ncld == 3 && strcmp((ExtDef->cld)[1]->name, "FunDec")==0)
    {
        Quadruple code1 = translate_FunDec((ExtDef->cld)[1]);
        Quadruple code2 = translate_CompSt((ExtDef->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

Quadruple translate_FunDec(SyntaxTreeNode FunDec)
{
    if (FunDec->ncld == 4)
    {
        Operand function = new_Variable((FunDec->cld)[0]->content);
        Quadruple code1 = new_Code();
        code1->operation="FUNCTION";
        code1->operands.var = function;
        Quadruple code2 = translate_VarList((FunDec->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    else if (FunDec->ncld == 3)
    {
        Operand function = new_Variable((FunDec->cld)[0]->content);
        Quadruple code1 = new_Code();
        code1->operation="FUNCTION";
        code1->operands.var = function;
        return code1;
    }
    return new_Code();
}
Quadruple translate_VarList(SyntaxTreeNode VarList)
{
    if (VarList->ncld == 1)
    {
        return translate_ParamDec((VarList->cld)[0]);
    }
    else if (VarList->ncld == 3)
    {
        Quadruple code1 = translate_ParamDec((VarList->cld)[0]);
        Quadruple code2 = translate_VarList((VarList->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
Quadruple translate_ParamDec(SyntaxTreeNode ParamDec)
{
    if (ParamDec->ncld == 2)
    {
        SyntaxTreeNode ID = ((ParamDec->cld)[1]->cld)[0];
        Quadruple code1 = new_Code();
        code1->operation="PARAM";
        code1->operands.var = new_Variable(ID->content);
        return code1;
    }
    return new_Code();
}

Quadruple translate_CompSt(SyntaxTreeNode ComSt)
{
    if (ComSt->ncld == 4)
    {
        Quadruple code1 = translate_DefList((ComSt->cld)[1]);
        Quadruple code2 = translate_StmtList((ComSt->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

Quadruple translate_StmtList(SyntaxTreeNode StmtList)
{
    if (StmtList->ncld == 2)
    {
        Quadruple code1 = translate_Stmt((StmtList->cld)[0]);
        Quadruple code2 = translate_StmtList((StmtList->cld)[1]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

Quadruple translate_Stmt(SyntaxTreeNode Stmt)
{
    if (Stmt->ncld == 2 && !strcmp((Stmt->cld)[1]->name, "SEMI"))
    {
        return translate_Exp((Stmt->cld)[0], NULL);
    }
    else if (Stmt->ncld == 1 && !strcmp((Stmt->cld)[0]->name, "Compst"))
    {
        return translate_CompSt((Stmt->cld)[0]);
    }
    else if (Stmt->ncld == 3 && !strcmp((Stmt->cld)[0]->name, "RETURN"))
    {
        Operand existOp = get_Operand((Stmt->cld)[1]);
        if (existOp == NULL)
        {
            Operand t1 = new_tempvar();
            Quadruple code1 = translate_Exp((Stmt->cld)[1], t1);
            Quadruple code2 = new_Code();
            code2->operation = "RETURN";
            code2->operands.var = t1;
            return add_Codes(2, code1, code2);
        }
        else
        {
            Quadruple code1 = new_Code();
            code1->operation = "RETURN";
            code1->operands.var = existOp;
            return code1;
        }
    }
    else if (Stmt->ncld == 5 && !strcmp((Stmt->cld)[0]->name, "IF"))
    {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        Quadruple code1 = translate_Cond((Stmt->cld)[2], lable1, lable2);
        Quadruple code2 = translate_Stmt((Stmt->cld)[4]);
        return add_Codes(4, code1, new_lable_Code(lable1), code2, new_lable_Code(lable2));
    }
    else if (Stmt->ncld == 7 && !strcmp((Stmt->cld)[0]->name, "IF"))
    {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        Operand lable3 = new_lable();
        Quadruple code1 = translate_Cond((Stmt->cld)[2], lable1, lable2);
        Quadruple code2 = translate_Stmt((Stmt->cld)[4]);
        Quadruple code3 = translate_Stmt((Stmt->cld)[6]);
        return add_Codes(7, code1, new_lable_Code(lable1), code2, new_goto_Code(lable3), new_lable_Code(lable2), code3, new_lable_Code(lable3));
    }
    else if (Stmt->ncld == 5 && !strcmp((Stmt->cld)[0]->name, "WHILE"))
    {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        Operand lable3 = new_lable();
        Quadruple code1 = translate_Cond((Stmt->cld)[2], lable2, lable3);
        Quadruple code2 = translate_Stmt((Stmt->cld)[4]);
        return add_Codes(6, new_lable_Code(lable1), code1, new_lable_Code(lable2), code2, new_goto_Code(lable1), new_lable_Code(lable3));
    }
    return new_Code();
}

Quadruple translate_DefList(SyntaxTreeNode DefList)
{
    if (DefList->ncld == 2)
    {
        Quadruple code1 = translate_Def((DefList->cld)[0]);
        Quadruple code2 = translate_DefList((DefList->cld)[1]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
Quadruple translate_Def(SyntaxTreeNode Def)
{
    if (Def->ncld == 3)
    {
        return translate_DecList((Def->cld)[1]);
    }
    return new_Code();
}
Quadruple translate_DecList(SyntaxTreeNode DecList)
{
    if (DecList->ncld == 1)
    {
        return translate_Dec((DecList->cld)[0]);
    }
    else if (DecList->ncld == 3)
    {
        Quadruple code1 = translate_Dec((DecList->cld)[0]);
        Quadruple code2 = translate_DecList((DecList->cld)[2]);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}
Quadruple translate_Dec(SyntaxTreeNode Dec)
{
    if (Dec->ncld == 3)
    {
        SyntaxTreeNode ID = ((Dec->cld)[0]->cld)[0];
        Operand variable = new_Variable(ID->content);
        Operand t1 = new_tempvar();
        Quadruple code1 = translate_Exp((Dec->cld)[2], t1);
        Quadruple code2 = new_assign_Code(variable, t1);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

Operand get_Operand(SyntaxTreeNode Exp)
{
    if (Exp->ncld == 1 && strcmp((Exp->cld)[0]->name, "INT")==0)
    {
        return find_Const((int)((Exp->cld)[0]->value));
    }
    else if (Exp->ncld == 1 && strcmp((Exp->cld)[0]->name, "ID")==0)
    {
        Operand variable = new_Variable((Exp->cld)[0]->content);
        return variable;
    }
    else if (Exp->ncld == 2 && strcmp((Exp->cld)[0]->name, "MINUS")==0)
    {
        if (!strcmp(((Exp->cld)[1]->cld)[0]->name, "INT"))
        {
            int value = -(int)(((Exp->cld)[1]->cld)[0]->value);
            Operand result = find_Const(value);
            if (result == NULL)
                return new_Const(value);
            else
                return result;
        }
    }
    return NULL;
}

Operand find_Const(int value)
{
    int i;
    for (i = 0; i < tNum-1; i++)
    {
        if (strcmp(temp_Operands[i]->type,"TEMPVAR")==0 && temp_Operands[i]->value == value)
            return temp_Operands[i];
    }
    return NULL;
}

Quadruple translate_Exp(SyntaxTreeNode Exp, Operand place)
{
    int isCond = 0;
    int flag1=0;
    int flag2=0;
    int flag3=0;
    int flag4=0;
    int flag5=0;
    int flag6=0;
    if(Exp->ncld > 1 ){
       flag1 = (strcmp((Exp->cld)[1]->name, "LT")==0);
       flag2 = (strcmp((Exp->cld)[1]->name, "LE")==0);
       flag3 = (strcmp((Exp->cld)[1]->name, "GT")==0);
       flag4 = (strcmp((Exp->cld)[1]->name, "GE")==0);
       flag5 = (strcmp((Exp->cld)[1]->name, "NE")==0);
       flag6 = (strcmp((Exp->cld)[1]->name, "EQ")==0);
    }
    int flag = flag1||flag2||flag3||flag4||flag5||flag6;
    if (Exp->ncld == 1 && strcmp((Exp->cld)[0]->name, "INT")==0)
    {
        Operand value = new_Const((Exp->cld)[0]->value);
        Quadruple code = new_assign_Code(place, value);
        return code;
    }
    else if (Exp->ncld == 1 && strcmp((Exp->cld)[0]->name, "ID")==0)
    {
        Operand variable = new_Variable((Exp->cld)[0]->content);
        Quadruple code = new_assign_Code(place, variable);
        return code;
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "ASSIGN")==0)
    {
        if ((Exp->cld)[0]->ncld == 1 && strcmp(((Exp->cld)[0]->cld)[0]->name, "ID")==0)
        {
            Operand variable = new_Variable(((Exp->cld)[0]->cld)[0]->content);
            Operand existOp = get_Operand((Exp->cld)[2]);
            if (existOp == NULL)
            {
                Operand t1 = new_tempvar();
                Quadruple code1 = translate_Exp((Exp->cld)[2], t1);
                Quadruple code2 = new_assign_Code(variable, t1);
                if (place == NULL)
                    return add_Codes(2, code1, code2);
                else
                {
                    Quadruple code3 = new_assign_Code(place, variable);
                    return add_Codes(3, code1, code2, code3);
                }
            }
            else
            {
                return new_assign_Code(variable, existOp);
            }
        }
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "PLUS")==0)
    {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL)
        {
            Quadruple code3 = new_Code();
            code3->operation="ADD";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        }
        else if (op1 == NULL && op2 != NULL)
        {
            Operand t1 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code3 = new_Code();
            code3->operation="ADD";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        }
        else if (op1 != NULL && op2 == NULL)
        {
            Operand t2 = new_tempvar();
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="ADD";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        }
        else
        {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="ADD";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "MINUS")==0)
    {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL)
        {
            Quadruple code3 = new_Code();
            code3->operation="SUB";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        }
        else if (op1 == NULL && op2 != NULL)
        {
            Operand t1 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code3 = new_Code();
            code3->operation="SUB";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        }
        else if (op1 != NULL && op2 == NULL)
        {
            Operand t2 = new_tempvar();
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="SUB";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        }
        else
        {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="SUB";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "MUL")==0)
    {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL)
        {
            Quadruple code3 = new_Code();
            code3->operation="MUL";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        }
        else if (op1 == NULL && op2 != NULL)
        {
            Operand t1 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code3 = new_Code();
            code3->operation="MUL";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        }
        else if (op1 != NULL && op2 == NULL)
        {
            Operand t2 = new_tempvar();
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="MUL";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        }
        else
        {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="MUL";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "DIV")==0)
    {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        if (op1 != NULL && op2 != NULL)
        {
            Quadruple code3 = new_Code();
            code3->operation="DIV";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = op2;
            return code3;
        }
        else if (op1 == NULL && op2 != NULL)
        {
            Operand t1 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code3 = new_Code();
            code3->operation="DIV";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = op2;
            return add_Codes(2, code1, code3);
        }
        else if (op1 != NULL && op2 == NULL)
        {
            Operand t2 = new_tempvar();
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="DIV";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = op1;
            code3->operands.binop.op2 = t2;
            return add_Codes(2, code2, code3);
        }
        else
        {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            Quadruple code3 = new_Code();
            code3->operation="DIV";
            code3->operands.binop.result = place;
            code3->operands.binop.op1 = t1;
            code3->operands.binop.op2 = t2;
            return add_Codes(3, code1, code2, code3);
        }
    }
    else if (Exp->ncld == 2 && strcmp((Exp->cld)[0]->name, "MINUS")==0)
    {
        Operand t1 = new_tempvar();
        Quadruple code1 = translate_Exp((Exp->cld)[1], t1);
        Quadruple code2 = new_Code();
        code2->operation="SUB";
        code2->operands.binop.result = place;
        code2->operands.binop.op1 = new_Const(0);
        code2->operands.binop.op2 = t1;
        return add_Codes(2, code1, code2);
    }
    else if (Exp->ncld == 3 && flag || Exp->ncld == 2 && !strcmp((Exp->cld)[0]->name, "NOT") || Exp->ncld == 3 && !strcmp((Exp->cld)[1]->name, "AND") || Exp->ncld == 3 && !strcmp((Exp->cld)[1]->name, "OR"))
    {
        isCond = 1;
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "LP")==0)
    {
        Operand function = new_Operand();
        function->type="FUNC";
        function->operand.name = (Exp->cld)[0]->content;
        if (!strcmp(function->operand.name, "read"))
        {
            Quadruple code = new_Code();
            code->operation="READ";
            code->operands.var = place;
            return code;
        }
        else
        {
            Quadruple code = new_Code();
            code->operation="CALL";
            code->operands.assign.left = place;
            code->operands.assign.right = function;
            return code;
        }
    }
    else if (Exp->ncld == 4 && strcmp((Exp->cld)[2]->name, "Args")==0)
    {
        int i;
        Operand function = new_Operand();
        function->type = "FUNC";
        function->operand.name = (Exp->cld)[0]->content;
        ArgList arg_list = (ArgList)malloc(sizeof(ArgListStru));
        arg_list->num = 0;
        Quadruple code1 = translate_Args((Exp->cld)[2], arg_list);
        Quadruple code2, code3;
        if (strcmp(function->operand.name, "write")==0)
        {
            code2 = new_Code();
            code2->operation="WRITE";
            code2->operands.var = (arg_list->list)[0];
            return add_Codes(2, code1, code2);
        }
        else
        {
            for (i = 0; i < arg_list->num; i++)
            {
                code2 = new_Code();
                code2->operation="ARG";
                code2->operands.var = (arg_list->list)[i];
                code1 = add_Codes(2, code1, code2);
            }
            code3 = new_Code();
            code3->operation="CALL";
            code3->operands.assign.left = place;
            code3->operands.assign.right = function;
            return add_Codes(2, code1, code3);
        }
    }else if(Exp->ncld == 3 && strcmp((Exp->cld)[0]->name, "LP")==0 && strcmp((Exp->cld)[2]->name, "RP")==0){
       Quadruple code = translate_Exp((Exp->cld)[1], place);
       return code;
   }
    else
    {
        printf("error\n");
        printf("%s\n",(Exp->cld)[0]->name);
        printf("%s\n",(Exp->cld)[1]->name);
        printf("%s\n",(Exp->cld)[2]->name); 
    }
    if (isCond)
    {
        Operand lable1 = new_lable();
        Operand lable2 = new_lable();
        Quadruple code0 = new_assign_Code(place, new_Const(0));
        Quadruple code1 = translate_Cond(Exp, lable1, lable2);
        Quadruple code2 = add_Codes(2, new_lable_Code(lable1), new_assign_Code(place, new_Const(1)));
        return add_Codes(4, code0, code1, code2, new_lable_Code(lable2));
    }
    return new_Code();
}

Quadruple translate_Cond(SyntaxTreeNode Exp, Operand lable_true, Operand lable_false)
{
    int flag1=0;
    int flag2=0;
    int flag3=0;
    int flag4=0;
    int flag5=0;
    int flag6=0;
    if(Exp->ncld > 1 ){
       flag1 = (strcmp((Exp->cld)[1]->name, "LT")==0);
       flag2 = (strcmp((Exp->cld)[1]->name, "LE")==0);
       flag3 = (strcmp((Exp->cld)[1]->name, "GT")==0);
       flag4 = (strcmp((Exp->cld)[1]->name, "GE")==0);
       flag5 = (strcmp((Exp->cld)[1]->name, "NE")==0);
       flag6 = (strcmp((Exp->cld)[1]->name, "EQ")==0);
    }
    int flag = flag1||flag2||flag3||flag4||flag5||flag6;

    if (Exp->ncld == 3 && flag)
    {
        Operand op1 = get_Operand((Exp->cld)[0]);
        Operand op2 = get_Operand((Exp->cld)[2]);
        Quadruple code3 = new_Code();
        code3->operation="IFGOTO";
        code3->operands.jump.lable = lable_true;
        code3->operands.jump.relop = (Exp->cld)[1]->content;
        if (op1 == NULL && op2 == NULL)
        {
            Operand t1 = new_tempvar();
            Operand t2 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            code3->operands.jump.op1 = t1;
            code3->operands.jump.op2 = t2;
            return add_Codes(4, code1, code2, code3, new_goto_Code(lable_false));
        }
        else if (op1 == NULL && op2 != NULL)
        {
            Operand t1 = new_tempvar();
            Quadruple code1 = translate_Exp((Exp->cld)[0], t1);
            code3->operands.jump.op1 = t1;
            code3->operands.jump.op2 = op2;
            return add_Codes(3, code1, code3, new_goto_Code(lable_false));
        }
        else if (op1 != NULL && op2 == NULL)
        {
            Operand t2 = new_tempvar();
            Quadruple code2 = translate_Exp((Exp->cld)[2], t2);
            code3->operands.jump.op1 = op1;
            code3->operands.jump.op2 = t2;
            return add_Codes(3, code2, code3, new_goto_Code(lable_false));
        }
        else if (op1 != NULL && op2 != NULL)
        {
            code3->operands.jump.op1 = op1;
            code3->operands.jump.op2 = op2;
            return add_Codes(2, code3, new_goto_Code(lable_false));
        }
    }
    else if (Exp->ncld == 2 && strcmp((Exp->cld)[0]->name, "NOT")==0)
    {
        return translate_Cond((Exp->cld)[1], lable_false, lable_true);
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "AND")==0)
    {
        Operand lable1 = new_lable();
        Quadruple code1 = translate_Cond((Exp->cld)[0], lable1, lable_false);
        Quadruple code2 = translate_Cond((Exp->cld)[2], lable_true, lable_false);
        return add_Codes(3, code1, new_lable_Code(lable1), code2);
    }
    else if (Exp->ncld == 3 && strcmp((Exp->cld)[1]->name, "OR")==0)
    {
        Operand lable1 = new_lable();
        Quadruple code1 = translate_Cond((Exp->cld)[0], lable_true, lable1);
        Quadruple code2 = translate_Cond((Exp->cld)[2], lable_true, lable_false);
        return add_Codes(3, code1, new_lable_Code(lable1), code2);
    }
    else
    {
        Operand t1 = new_tempvar();
        Quadruple code1 = translate_Exp(Exp, t1);
        Quadruple code2 = new_Code();
        char *relop = "!=";
        code2->operation="IFGOTO";
        code2->operands.jump.lable = lable_true;
        code2->operands.jump.relop = relop;
        code2->operands.jump.op1 = t1;
        code2->operands.jump.op2 = new_Const(0);
        return add_Codes(3, code1, code2, new_goto_Code(lable_false));
    }
}
Quadruple translate_Args(SyntaxTreeNode Args, ArgList arg_list)
{
    if (Args->ncld == 1)
    {
        Operand existOp = get_Operand((Args->cld)[0]);
        if (existOp != NULL)
        {
            if (strcmp("CONSTANT",existOp->type)==0)
            {
                Operand t1 = new_tempvar();
                Quadruple code1 = new_assign_Code(t1, existOp);
                arg_list->list[arg_list->num] = t1;
                arg_list->num++;
                return code1;
            }
            arg_list->list[arg_list->num] = existOp;
            arg_list->num++;
            return new_Code();
        }
        Operand t1 = new_tempvar();
        Quadruple code1 = translate_Exp((Args->cld)[0], t1);
        arg_list->list[arg_list->num] = t1;
        arg_list->num++;
        return code1;
    }
    else if (Args->ncld == 3)
    {
        Operand t1 = new_tempvar();
        Quadruple code1 = translate_Exp((Args->cld)[0], t1);
        arg_list->list[arg_list->num] = t1;
        arg_list->num++;
        Quadruple code2 = translate_Args((Args->cld)[2], arg_list);
        return add_Codes(2, code1, code2);
    }
    return new_Code();
}

