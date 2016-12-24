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
struct symEntry *find_ID_Decl(struct symTable *table,char *name);
int check_Func_Type_and_Param_equal(struct symEntry *entry,char *name,struct Type *type,struct Param_list *param_list);
void check_Func_define(struct symTable *S_table);
