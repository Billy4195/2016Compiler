#include "DataType.h"
#include "symbolTable.h"
#include <string.h>

struct symEntry *find_ID_Decl(struct symTable *table,char *name){
    int cur=0;
    for(;cur < table->filled;cur++){
        if(strcmp(table->table[cur]->name,name) == 0){
            return table->table[cur];
        }
    }
    return NULL;
}
