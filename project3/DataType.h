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
    struct Type *type;
    char *name;
    struct Param *next;
};

struct Param_list{
    struct Param *head;
    struct Param *tail;
};

struct Const_type{
    char *name;
    struct ConstAttr *attr;
    struct Const_type *next;
};

struct Const_list{
    struct Const_type *head;
    struct Const_type *tail;
};

union Attr{
    struct ConstAttr *constVal;
    struct Param_list *param_list;
};

struct symEntry{
    char *name;
    Kind_t kind;
    int level;
    struct Type *type;
    union Attr *attr;
    __BOOL_t isDef;
};
#endif
