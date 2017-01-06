#include "header.h"
#include "semcheck.h"
#include "symtab.h"

void add_main();
void add_method(const char *id,struct param_sem *params,struct PType *retType);
char *trans_type(struct PType *type);
