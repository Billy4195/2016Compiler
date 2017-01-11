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
    int count=0,targe_index;
    struct SymNode *nodePtr,*target;
    for( nodePtr = table->entry[0] ; nodePtr != 0;nodePtr= nodePtr->next){
        if(!strcmp(nodePtr->name,var->varRef->id)){
            target = nodePtr;
            targe_index = count;
        }
        if(nodePtr->scope != 0){
            count++;
        }
    }
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
        fprintf(ofp,"%d\n",targe_index);
    }else{
        printf("variable %s not found\n",var->varRef->id);
    }
}

void neg_op(struct expr_sem *var){
    switch(var->pType->type){
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
    }
}
