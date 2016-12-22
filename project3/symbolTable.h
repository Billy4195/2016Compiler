#include "DataType.h"

struct ConstAttr *new_ConstAttr(Kind_t kind, void *value);
struct Type *new_Type(Kind_t kind);
struct Dim *new_Dim(int num);
void Dim_add_new_num(struct Dim *dim,int num);
