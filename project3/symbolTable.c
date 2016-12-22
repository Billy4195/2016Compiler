#include "DataType.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
