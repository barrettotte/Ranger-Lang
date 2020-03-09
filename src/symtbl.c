#include "ranger-lang.h"


// Get symbol index or -1 if not found
int findSymbol(char *s){
    for(int i = 0; i < g_NextFreeSymbol; i++){
        if(*s == *g_Symtbl[i].name && !strcmp(s, g_Symtbl[i].name)){
            return i;
        }
    }
    return -1;
}

// Get next free symbol index, die if too many symbols used
static int newSymbol(){
    int p;
    if((p = g_NextFreeSymbol++) >= SYMTBL_LEN){
        fatal("Too many symbols in use");
    }
    return p;
}

// Add symbol to symbol table, return its index
int addSymbol(char *name){
    int x;
    if((x = findSymbol(name)) != -1){
        return x; // return symbol if already exists
    }
    x = newSymbol();
    g_Symtbl[x].name = strdup(name);
    return x;
}
