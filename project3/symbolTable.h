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
