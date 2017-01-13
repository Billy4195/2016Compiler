#include "header.h"
#include "semcheck.h"
#include "symtab.h"

void add_main();
void insert_main_param(struct SymTable *table,int scope);
void add_method(const char *id,struct param_sem *params,struct PType *retType);
void add_global_var(const char *id,struct PType *type);
void load_var(struct SymTable *table,struct expr_sem *var);
void store_var(struct SymTable *table,char *id,struct PType *type);
void load_int(int i);
void load_float(float f);
void load_double(double d);
void load_str(char *str);
void neg_op(struct expr_sem *expr);
void mod_op();
void mul_op(struct expr_sem *expr,int op);
void add_op(struct expr_sem *expr,int op);
void or_op();
void and_op();
void not_op();
void invoke_print(struct expr_sem *expr);
void func_invoke(struct SymTable *table,char *name,int needpop);
void return_op(struct expr_sem *expr);
char *trans_type(struct PType *type);
struct SymNode *find_symbol(struct SymTable *table,char *name,int *target_index);
