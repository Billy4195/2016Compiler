#ifndef Data
#define Data
typedef enum { VOID_t, INT_t, FLOAT_t, DOUBLE_t, BOOLEAN_t, ARRAY_t, STR_t, CONST_t, PARAM_t, FUNC_t, VAR_t} Kind_t;

typedef enum { __TRUE, __FALSE } __BOOL_t;

struct Dim{
    unsigned int capacity;
    unsigned int filled;
    unsigned int *numbers;
};

struct ID_type{
    char *name;
    struct Dim *dim;
    struct ID_type *next;
};

struct ID_list{
    struct ID_type *head;
    struct ID_type *tail;
};

struct Type{
    Kind_t kind;
    __BOOL_t isArray;
    struct Dim *dim;
};

struct ConstAttr{
    Kind_t kind;
    union {
        int intval;
        float floval;
        double douval;
        char *strval;
        __BOOL_t bval;
    }value;
    __BOOL_t minus;
};

struct Param{
    char *name;
    struct Type *type;
};

/*struct ParamList{
    
};

struct FuncAttr{
    int len;

};*/

union{
    struct ConstAttr *constVal;
    struct FuncAttr *param_list;
}Attr;

struct symEntry{
    char *name;
    Kind_t kind;
    int level;
    struct Type *type;
    struct Attr *attr;
    __BOOL_t isDef;
};
#endif
