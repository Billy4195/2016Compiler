#include "header.h"
#include "semcheck.h"
#include "symtab.h"

void add_main();
void add_method(const char *id,struct param_sem *params,struct PType *retType);
void add_global_var(const char *id,struct PType *type);
char *trans_type(struct PType *type);
