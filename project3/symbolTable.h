#include "DataType.h"

struct ConstAttr *new_ConstAttr(Kind_t kind, void *value);
struct Type *new_Type(Kind_t kind);
struct Dim *new_Dim(int num);
void Dim_add_new_num(struct Dim *dim,int num);
struct ID_type *new_ID(char *name,struct Dim *dim);
struct ID_list *new_ID_list(struct ID_type *id);
void ID_list_push_back(struct ID_list *list, struct ID_type *id);
struct Param *new_Param(struct Type *type, struct ID_type *id);
struct Param_list *new_Param_list(struct Param *param);
void Param_list_push_back(struct Param_list *list, struct Param *param);
struct Const_type* new_Const(char *name,struct ConstAttr *attr);
struct Const_list* new_Const_list(struct Const_type *con);
void Const_list_push_back(struct Const_list *list, struct Const_type *con);
struct symEntry *createFunc_node(struct Type *type, char *name, struct Param_list *param_list, int level, __BOOL_t isDef);
void init_Table(struct symTable *symtable);
