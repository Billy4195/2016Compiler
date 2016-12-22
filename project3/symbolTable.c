#include "DataType.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_Table(struct symTable *symboltable){
    symboltable->table = (struct symEntry**)malloc(sizeof(struct symEntry*) * 15);
    symboltable->capacity = 15;
    symboltable->filled = 0;
}

void Table_push_back(struct symTable *symboltable, struct symEntry *entry){
    if(symboltable->capacity == symboltable->filled){
        struct symEntry **tmp = (struct symEntry **)malloc(sizeof(struct symEntry*) * (symboltable->capacity + 5));
        memcpy(tmp,symboltable->table,sizeof(struct symEntry*)*symboltable->capacity);
        free(symboltable->table);
        symboltable->table = tmp;
        symboltable->capacity += 5;
    }
    symboltable->table[symboltable->filled++] = entry;
}

struct ConstAttr *new_ConstAttr(Kind_t kind, void *value){
    struct ConstAttr *new = (struct ConstAttr*) malloc(sizeof(struct ConstAttr));
    new->kind = kind;
    switch(kind){
    case INT_t:{
        int tmp = *(int *)value;
        if(tmp < 0){
            new->value.intval = -tmp;
            new->minus = __TRUE;
        }else{
            new->value.intval = tmp;
            new->minus = __FALSE;
        }
        break;
    }
    case FLOAT_t:{
        float tmp = *(float *)value;
        if(tmp < 0){
            new->value.floval = -tmp;
            new->minus = __TRUE;
        }else{
            new->value.floval = tmp;
            new->minus = __FALSE;
        }
        break;
    }
    case DOUBLE_t:{
        double tmp = *(double *)value;
        if(tmp < 0){
            new->value.douval = -tmp;
            new->minus = __TRUE;
        }else{
            new->value.douval = tmp;
            new->minus = __FALSE;
        }
        break;
    }
    case STR_t:{
        new->value.strval = (char*)value;
        break;
    }
    case BOOLEAN_t:{
        new->value.bval = *(int*)value;
        break;
    }
    }
    return new;
}

struct Type *new_Type(Kind_t kind){
    struct Type *new = (struct Type*)malloc(sizeof(struct Type));
    new->kind = kind;
    new->isArray = __FALSE;
    new->dim = NULL;

    return new;
}

struct Dim *new_Dim(int num){
    struct Dim *new = (struct Dim*)malloc(sizeof(struct Dim));
    new->numbers = (unsigned int*)malloc(sizeof(unsigned int) * 2);
    new->capacity = 2;
    new->filled = 0;

    new->numbers[new->filled++] = num;
    return new;
}

void Dim_add_new_num(struct Dim *dim,int num){
    if(dim->capacity == dim->filled){
        unsigned int *tmp = (unsigned int*)malloc(sizeof(unsigned int) * (dim->capacity + 2));
        memcpy(tmp,dim->numbers,sizeof(unsigned int)*dim->capacity);
        free(dim->numbers);
        dim->numbers = tmp;
        dim->capacity += 2;
    }
    dim->numbers[dim->filled++] = num;
}

struct ID_type *new_ID(char *name,struct Dim *dim){
    struct ID_type *new = (struct ID_type*)malloc(sizeof(struct ID_type));
    new->name = name;
    new->dim = dim;
    new->next = NULL;
    return new;
}

struct ID_list *new_ID_list(struct ID_type *id){
    struct ID_list *new = (struct ID_list*)malloc(sizeof(struct ID_list));
    new->head = id;
    new->tail = id;
    return new;
}

void ID_list_push_back(struct ID_list *list, struct ID_type *id){
    list->tail->next = id;
    list->tail = id;
}

struct Param *new_Param(struct Type *type, struct ID_type *id){
    struct Param *new = (struct Param*)malloc(sizeof(struct Param));
    new->type = type;
    new->type->isArray = id->dim != NULL ? __TRUE : __FALSE;
    new->type->dim = id->dim;       //no matter NULL or not
    new->name = id->name;
    new->next = NULL;
    free(id);
    return new;
}

struct Param_list *new_Param_list(struct Param *param){
    struct Param_list *new = (struct Param_list*)malloc(sizeof(struct Param_list));
    new->head = param;
    new->tail = param;
    return new;
}

void Param_list_push_back(struct Param_list *list, struct Param *param){
    list->tail->next = param;
    list->tail = param;
}

struct Const_type* new_Const(char *name,struct ConstAttr *attr){
    struct Const_type *new = (struct Const_type*)malloc(sizeof(struct Const_type));
    new->name = name;
    new->attr = attr;
    new->next = NULL;
    return new;
}

struct Const_list* new_Const_list(struct Const_type *con){
    struct Const_list *new = (struct Const_list*)malloc(sizeof(struct Const_list));
    new->head = con;
    new->tail = con;
    return new;
}

void Const_list_push_back(struct Const_list *list, struct Const_type *con){
    list->tail->next = con;
    list->tail = con;
}

struct symEntry *createFunc_node(struct Type *type,char *name,struct Param_list *param_list, int level, __BOOL_t isDef){
    struct symEntry *new = (struct symEntry*)malloc(sizeof(struct symEntry));
    new->type = type;
    new->name = name;
    new->attr = (union Attr*)malloc(sizeof(union Attr));
    new->attr->param_list = param_list;
    new->level = level;
    new->isDef = isDef;
    new->kind = FUNC_t;
    return new;
}

void print_Type(struct Type *type,int fixed){
    char buffer[30];
    memset(buffer,0,sizeof(buffer));
    switch(type->kind){
    case INT_t:
        sprintf(buffer,"int");
        break;
    case FLOAT_t:
        sprintf(buffer,"float");
        break;
    case DOUBLE_t:
        sprintf(buffer,"double");
        break;
    case STR_t:
        sprintf(buffer,"string");
        break;
    case BOOLEAN_t:
        sprintf(buffer,"bool");
        break;
    case VOID_t:
        sprintf(buffer,"void");
        break;
    }
    if(type->isArray){
        int i;
        char b[15];
        for(i=0;i<type->dim->filled;i++){
            memset(b,0,sizeof(b));
            sprintf(b,"[%d]",type->dim->numbers[i]);
            strcat(buffer,b);
        }
    }
    if(fixed)
        printf("%-19s",buffer);
    else
        printf("%s",buffer);
}

void print_Table(struct symTable *symboltable,int scope){
    int i;
    printf("=======================================================================================\n");
    // Name [29 blanks] Kind [7 blanks] Level [7 blank] Type [15 blanks] Attribute [15 blanks]
    printf("Name                             Kind       Level       Type               Attribute               \n");
    printf("---------------------------------------------------------------------------------------\n");
    for(i=0;i<symboltable->filled;i++){
        if(symboltable->table[i]->level == scope){
            printf("%-32s ",symboltable->table[i]->name);
            switch(symboltable->table[i]->kind){
            case FUNC_t:
                printf("function  ");
                break;
            case VAR_t:
                printf("variable  ");
                break;
            case PARAM_t:
                printf("parameter ");
                break;
            case CONST_t:
                printf("constant  ");
                break;
            }
            if(symboltable->table[i]->level == 0){
                printf("%2d(global)   ",symboltable->table[i]->level);
            }else{
                printf("%2d(local)    ",symboltable->table[i]->level);
            }
            print_Type( symboltable->table[i]->type, 1);
            if(symboltable->table[i]->kind == CONST_t){
                struct ConstAttr *tmp = symboltable->table[i]->attr->constVal;
                switch(tmp->kind){
                case INT_t:
                    if(tmp->minus){
                        printf("%d",-tmp->value.intval);
                    }else{
                        printf("%d",tmp->value.intval);
                    }
                    break;
                case FLOAT_t:
                    if(tmp->minus){
                        printf("%f",-tmp->value.floval);
                    }else{
                        printf("%f",tmp->value.floval);
                    }
                    break;
                case DOUBLE_t:
                    if(tmp->minus){
                        printf("%lf",-tmp->value.douval);
                    }else{
                        printf("%lf",tmp->value.douval);
                    }
                    break;
                case STR_t:
                    printf("%s",tmp->value.strval);
                    break;
                case BOOLEAN_t:
                    if(tmp->value.bval){
                        printf("True");
                    }else{
                        printf("False");
                    }
                    break;
                }
            }else if(symboltable->table[i]->kind == FUNC_t){
                struct Param_list *tmp = symboltable->table[i]->attr->param_list;
                if(tmp){
                    struct Param *it = tmp->head;
                    if(it){
                        print_Type( it->type, 0);
                        it = it->next;
                    }
                    for(; it != NULL ;it = it->next){
                        printf(",");
                        print_Type( it->type, 0 );
                    }
                }
            }
            printf("\n");
        }
    }
    printf("======================================================================================\n");
}

