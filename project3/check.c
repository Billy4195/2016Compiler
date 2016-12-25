#include "DataType.h"
#include "symbolTable.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int linenum;
extern int error_happened;

char *new_message(){
    char *message = (char *)malloc(sizeof(char)*80);
    memset(message,0,sizeof(char)*80);
    return message;
}

void print_error(char *str1, char *name, char *str2){
    char *message = new_message();
    strcat(message,str1);
    strcat(message,name);
    strcat(message,str2);
    printf("########## Error at Line#%d: %s ##########\n",linenum,message);
    free(name);
    free(message);
    error_happened |= 1;
}

void Redeclaration(char *name){
    print_error("symbol ",name," is redeclared");
}

void Redefinition(char *name){
    print_error("The function ",name," is redefined");
}

void Name_reuse(char *name){
    print_error("The name ",name," is used");
}

void Func_incompatible(char *name){
    linenum--;
    print_error("The function ",name," is incompatible with previous declaration");
    linenum++;
}

void Array_index_error(char *name){
    print_error("The dimension of array ",name," in declaration should be greater than 0");
}

void Not_func_invoke(char *name){
    print_error("The invocation of ",name," is not Function type");
}

void Func_invoke_not_decl_or_def(char *name){
    print_error("The function ",name," should be declared or defined before being used");
}

void Func_no_def(char *name){
    print_error("The function ",name," is declared but not defined");
}

void Undef_reference(char *name){
    print_error("Undefined reference to \'",name,"\'");
}

void Expression_operand_error(char *op){
    print_error("The operand(s) of the operator ",strdup(op)," is wrong");
}

void Func_reference(char *name){
    print_error("Function ",name," can't be reference as a variable");
}

void Not_array_reference(char *name){
    print_error("The symbol ",name," is not array type");
}

void Const_assign(char *name){
    print_error("The assign of a const \"",strdup(name),"\" is not allowed");
}

void Array_over_subscripted(char *name){
    print_error("The dimesion to subscript array ",name," is unmatch to its declaration");
}

struct symEntry *find_ID_Decl(struct symTable *table,char *name){
    int cur=table->filled-1;
    for(;cur >= 0;cur--){
        if(strcmp(table->table[cur]->name,name) == 0){
            return table->table[cur];
        }
    }
    return NULL;
}

int check_Func_Type_and_Param_equal(struct symEntry *entry,char *name,struct Type *type,struct Param_list *param_list){
    if(entry->kind != FUNC_t){
        delete_Type(type); 
        Name_reuse(name);
        return 1;
    }
    if(entry->isDef){
        delete_Type(type); 
        Redefinition(name);
        return 2;
    }
    if(entry->type->kind == type->kind){
        if(Param_list_equal(entry->attr->param_list,param_list)){
            delete_Type(type); 
            return 0;
        }
    }
    delete_Type(type); 
    Func_incompatible(name);
    return 3;
}

void check_Func_define(struct symTable *S_table){
    int cur;
    for(cur=0;cur < S_table->filled;cur++){
        if(S_table->table[cur]->kind == FUNC_t){
            if(S_table->table[cur]->isDef == __FALSE){
                Func_no_def(S_table->table[cur]->name);
            }
        }
    }
}

struct ConstAttr *check_boolean_attr(struct ConstAttr *attr){
    if(!attr || attr->kind != BOOLEAN_t){
        return NULL;
    }else{
        return attr;
    }
}

struct ConstAttr *check_arithmetic_attr(struct ConstAttr *attr){
    if(!attr || (attr->kind != INT_t && attr->kind != FLOAT_t && attr->kind != DOUBLE_t)){
        return NULL;
    }else{
        return attr;
    }
}

struct ConstAttr *check_logical_operand(struct ConstAttr *op1,struct ConstAttr *op2){
    if(check_boolean_attr(op1) == NULL){
        return NULL;
    }
    if(check_boolean_attr(op2) == NULL){
        return NULL;
    }
    return new_ConstAttr(BOOLEAN_t,NULL,__FALSE);
}

struct ConstAttr *check_relation_operand(struct ConstAttr *operand1,char *operator,struct ConstAttr *operand2){
    if( (check_boolean_attr(operand1) || check_arithmetic_attr(operand1)) && (check_boolean_attr(operand2) || check_arithmetic_attr(operand2))){
        if(strcmp(operator,"==") == 0 || strcmp(operator,"!=") == 0){  //EQ or NE
            return new_ConstAttr(BOOLEAN_t,NULL,__FALSE);
        }else if(check_arithmetic_attr(operand2)){
            return new_ConstAttr(BOOLEAN_t,NULL,__FALSE);
        }
    }
    return NULL;
}

struct ConstAttr *check_arithmetic_operand(struct ConstAttr *operand1,char *operator,struct ConstAttr *operand2){
    if(check_arithmetic_attr(operand1) && check_arithmetic_attr(operand2)){
        if(operand1->kind == INT_t && operand2->kind == INT_t){
            return new_ConstAttr(INT_t,NULL,__FALSE);
        }else if(operand1->kind == DOUBLE_t || operand2->kind == DOUBLE_t){
            if(strcmp(operator,"%")){ // != %
                return new_ConstAttr(DOUBLE_t,NULL,__FALSE);
            }else{
                return NULL;
            }
        }else{
            if(strcmp(operator,"%")){       // != %
                return new_ConstAttr(FLOAT_t,NULL,__FALSE);
            }else{
                return NULL;
            }
        }
    }
    return NULL;
}

void check_const_assign(struct symEntry *node){
    if(node && node->kind == CONST_t){
        Const_assign(node->name);
    }
}
