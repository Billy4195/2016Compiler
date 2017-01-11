#include "header.h"
#include "semcheck.h"
#include "symtab.h"

void add_main();
void insert_main_param(struct SymTable *table,int scope);
void add_method(const char *id,struct param_sem *params,struct PType *retType);
void add_global_var(const char *id,struct PType *type);
void load_var(struct SymTable *table,struct expr_sem *var);
void neg_op(struct expr_sem *expr);
void mod_op();
void mul_op(struct expr_sem *expr,int op);
void add_op(struct expr_sem *expr,int op);
void or_op();
void and_op();
void not_op();
char *trans_type(struct PType *type);
