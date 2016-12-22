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
