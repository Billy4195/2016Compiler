#include "DataType.h"
#include "symbolTable.h"

void print_error(char *message);
void Redeclaration(char *name);
void Redefinition(char *name);
void Name_reuse(char *name);
void Func_incompatible(char *name);
void Array_index_error(char *name);
struct symEntry *find_ID_Decl(struct symTable *table,char *name);
int check_Func_Type_and_Param_equal(struct symEntry *entry,char *name,struct Type *type,struct Param_list *param_list);
