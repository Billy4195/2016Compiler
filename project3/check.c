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

struct symEntry *find_ID_Decl(struct symTable *table,char *name){
    int cur=0;
    for(;cur < table->filled;cur++){
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
