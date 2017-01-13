#include <stdio.h>
#include "header.h"
#include "semcheck.h"
#include "symtab.h"
#include "codegen.h"

extern FILE *ofp;
extern char *class_name;

void add_main(){
    fprintf(ofp,".method public static main([Ljava/lang/String;)V\n");
    fprintf(ofp,".limit stack 100\n");
    fprintf(ofp,".limit locals 100\n");
    fprintf(ofp,"   new java/util/Scanner\n");
    fprintf(ofp,"   dup\n");
    fprintf(ofp,"   getstatic java/lang/System/in Ljava/io/InputStream;\n");
    fprintf(ofp,"   invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
    fprintf(ofp,"   putstatic %s/_sc Ljava/util/Scanner;\n",class_name);
}

void insert_main_param(struct SymTable *table,int scope){
    struct SymNode *newNode;
    newNode = createParamNode(" ",scope,createPType(STRING_t));
    insertTab( table, newNode );
}

void add_method(const char *id,struct param_sem *params,struct PType *retType){
    struct param_sem *cur = params;
    fprintf(ofp,".method public static %s(",id); 
    if(params){
        while(cur != NULL){
            fprintf(ofp,"%s",trans_type(cur->pType));
            cur = cur->next;
        }
    }
    fprintf(ofp,")");
    fprintf(ofp,"%s\n",trans_type(retType));
    fprintf(ofp,".limit stack 100\n");
    fprintf(ofp,".limit locals 100\n");
}

void add_global_var(const char *id,struct PType *type){
    fprintf(ofp,".field public static %s %s\n",id,trans_type(type));
}

void load_var(struct SymTable *table,struct expr_sem *var){
    int target_index;
    struct SymNode *target=find_symbol(table,var->varRef->id,&target_index);
    if(target && target->scope == 0){
        fprintf(ofp,"   getstatic %s/%s %s\n",class_name,target->name,trans_type(target->type));
    }else if(target){
        switch(target->type->type){
        case INTEGER_t:
            fprintf(ofp,"   iload ");
            break;
        case BOOLEAN_t:
            fprintf(ofp,"   iload ");
            break;
        case FLOAT_t:
            fprintf(ofp,"   fload ");
            break;
        case DOUBLE_t:
            fprintf(ofp,"   dload ");
            break;
        }
        fprintf(ofp,"%d\n",target_index);
    }else{
        printf("variable %s not found\n",var->varRef->id);
    }
}

void store_var(struct SymTable *table,char *id,struct PType *type){
    int target_index;
    struct SymNode *target=find_symbol(table,id,&target_index);
    if(target && target->scope == 0){
        fprintf(ofp,"   putstatic %s/%s %s\n",class_name,id,trans_type(type));
    }else if(target){
        switch(target->type->type){
        case INTEGER_t:
            fprintf(ofp,"   istore ");
            break;
        case BOOLEAN_t:
            fprintf(ofp,"   istore ");
            break;
        case FLOAT_t:
            fprintf(ofp,"   fstore ");
            break;
        case DOUBLE_t:
            fprintf(ofp,"   dstore ");
            break;
        }
        fprintf(ofp,"%d\n",target_index);
    }else{
        printf("variable %s not found\n",id);
    }
}

void load_int(int i){
    fprintf(ofp,"   ldc %d\n",i);
}

void load_float(float f){
    fprintf(ofp,"   ldc %f\n",f);
}

void load_double(double d){
    fprintf(ofp,"   ldc %lf\n",d);
}

void load_str(char *str){
    if(strcmp(str,"\n")){
        fprintf(ofp,"   ldc \"%s\"\n",str);
    }else{
        fprintf(ofp,"   ldc \"\\n\"\n");
    }
}

void neg_op(struct expr_sem *expr){
    switch(expr->pType->type){
    case INTEGER_t:
        fprintf(ofp,"   ineg\n");
        break;
    case FLOAT_t:
        fprintf(ofp,"   fneg\n");
        break;
    case DOUBLE_t:
        fprintf(ofp,"   dneg\n");
        break;
    }
}

void mod_op(){
    fprintf(ofp,"   irem\n");
}

void mul_op(struct expr_sem *expr,int op){
    switch(expr->pType->type){
    case INTEGER_t:
        if(op == DIV_t){
            fprintf(ofp,"   idiv\n");
        }else{
            fprintf(ofp,"   imul\n");
        }
        break;
    case FLOAT_t:
        if(op == DIV_t){
            fprintf(ofp,"   fdiv\n");
        }else{
            fprintf(ofp,"   fmul\n");
        }
        break;
    case DOUBLE_t:
        if(op == DIV_t){
            fprintf(ofp,"   ddiv\n");
        }else{
            fprintf(ofp,"   dmul\n");
        }
        break;
    }
}

void add_op(struct expr_sem *expr,int op){
    switch(expr->pType->type){
    case INTEGER_t:
        if(op == ADD_t){
            fprintf(ofp,"   iadd\n");
        }else{
            fprintf(ofp,"   isub\n");
        }
        break;
    case FLOAT_t:
        if(op == ADD_t){
            fprintf(ofp,"   fadd\n");
        }else{
            fprintf(ofp,"   fsub\n");
        }
        break;
    case DOUBLE_t:
        if(op == ADD_t){
            fprintf(ofp,"   dadd\n");
        }else{
            fprintf(ofp,"   dsub\n");
        }
        break;
    }
}

void or_op(){
    fprintf(ofp,"   ior\n");
}

void and_op(){
    fprintf(ofp,"   iand\n");
}

void not_op(){
    fprintf(ofp,"   ldc 1\n");
    fprintf(ofp,"   ixor\n");
}

void invoke_print(struct expr_sem *expr){
    fprintf(ofp,"   invokevirtual java/io/PrintStream/print(%s)V\n",trans_type(expr->pType));
}

void func_invoke(struct SymTable *table,char *name,int needpop){
    struct SymNode *nodePtr,*target=NULL;
    for( nodePtr = table->entry[0] ; nodePtr != 0;nodePtr= nodePtr->next){
        if(!strcmp(nodePtr->name,name)){
            target = nodePtr;
            break;
        }
        if(nodePtr->scope != 0){
            return ;
        }
    }
    if(target->category == FUNCTION_t){
        fprintf(ofp,"   invokestatic %s/%s(",class_name,target->name);
        int i;
        struct PTypeList *cur=target->attribute->formalParam->params;
        for(i=0;i<target->attribute->formalParam->paramNum;i++){
            fprintf(ofp,"%s",trans_type(cur->value));
            cur = cur->next;
        }
        fprintf(ofp,")%s\n",trans_type(target->type));
        if(needpop){
            fprintf(ofp,"   pop\n");
        }
    }
}

void return_op(struct expr_sem *expr){
    switch(expr->pType->type){
    case INTEGER_t:
        fprintf(ofp,"   ireturn\n");
        break;
    case FLOAT_t:
        fprintf(ofp,"   freturn\n");
        break;
    case DOUBLE_t:
        fprintf(ofp,"   dreturn\n");
        break;
    case BOOLEAN_t:
        fprintf(ofp,"   ireturn\n");
        break;
    }
}

char *trans_type(struct PType *type){
    switch(type->type){
    case VOID_t:
        return "V";
    case INTEGER_t:
        return "I";
    case BOOLEAN_t:
        return "Z";
    case FLOAT_t:
        return "F";
    case DOUBLE_t:
        return "D";
    case STRING_t:
        return "Ljava/lang/String;";
    }
}

struct SymNode *find_symbol(struct SymTable *table,char *name,int *target_index){
    int count=0;
    struct SymNode *nodePtr,*target=NULL;
    for( nodePtr = table->entry[0] ; nodePtr != 0;nodePtr= nodePtr->next){
        if(!strcmp(nodePtr->name,name)){
            target = nodePtr;
            *target_index = count;
        }
        if(nodePtr->scope != 0){
            count++;
        }
    }
    return target;
}
