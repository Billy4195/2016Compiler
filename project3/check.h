#include "DataType.h"
#include "symbolTable.h"

void print_error(char *message);
void Redeclaration(char *name);
void Redefinition(char *name);
void Name_reuse(char *name);
void Func_incompatible(char *name);
void Array_index_error(char *name);
void Not_func_invoke(char *name);
void Func_invoke_not_decl_or_def(char *name);
void Func_no_def(char *name);
void Undef_reference(char *name);
void Expression_operand_error(char *op);
void Func_reference(char *name);
void Not_array_reference(char *name);
void Const_assign(char *name);
void Array_over_subscripted(char *name);
void Array_init_oversize(char *name);
struct symEntry *find_ID_Decl(struct symTable *table,char *name);
int check_Func_Type_and_Param_equal(struct symEntry *entry,char *name,struct Type *type,struct Param_list *param_list);
void check_Func_define(struct symTable *S_table);
struct ConstAttr *check_boolean_attr(struct ConstAttr *attr);
struct ConstAttr *check_arithmetic_attr(struct ConstAttr *attr);
struct ConstAttr *check_logical_operand(struct ConstAttr *op1,struct ConstAttr *op2);
struct ConstAttr *check_relation_operand(struct ConstAttr *operand1,char *operator,struct ConstAttr *operand2);
struct ConstAttr *check_arithmetic_operand(struct ConstAttr *operand1,char *operator,struct ConstAttr *operand2);
void check_const_assign(struct symEntry *node);
void check_array_init(struct ID_type *id,int init_array);
