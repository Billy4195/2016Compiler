#include <stdio.h>
#include "header.h"
#include "semcheck.h"
#include "symtab.h"
#include "codegen.h"

extern FILE *ofp;

void add_main(){
    fprintf(ofp,".method public static main([Ljava/lang/String;)V\n");
    fprintf(ofp,".limit stack 100\n");
    fprintf(ofp,".limit locals 100\n");
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
