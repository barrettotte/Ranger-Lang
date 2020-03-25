// Chained hash table for storing symbols

#include "rangerlang.h"

#define PRIME     211
#define HASHSHIFT   4

// Why 211 ? Read about hashpjw - P.J. Weinberger's C compiler

// linked list of source line numbers where a symbol is referenced
typedef struct LineNumberListRecord{
    int lineno;
    struct LineNumberListRecord *next;
} *LineNumberList;


// linked bucket list for each symbol
typedef struct BucketListRecord{
    char *name;
    LineNumberList lineNumbers;
    int memoryAddr;
    struct BucketListRecord *next;
} *BucketList;


static BucketList hashTable[PRIME];


// hash generation function
static int hash(char *key){
    int temp = 0;
    int i = 0;
    while(key[i] != '\0'){
        temp = ((temp << HASHSHIFT) + key[i]) % PRIME;
        i++;
    }
    return temp;
}


// Insert new symbol into symbol table
void insertSymbol(char *name, int lineno, int memoryAddr){
    int hashed = hash(name);
    BucketList bl = hashTable[hashed];

    while((bl != NULL) && (strcmp(name, bl->name) != 0)){
        bl = bl->next;
    }
    if(bl == NULL){
        // Not in symbol table, create new record
        bl = (BucketList) malloc(sizeof(struct BucketListRecord));
        bl->name = name;
        bl->lineNumbers = (LineNumberList) malloc(sizeof(struct LineNumberListRecord));
        bl->lineNumbers->lineno = lineno;
        bl->memoryAddr = memoryAddr;
        bl->lineNumbers->next = NULL;
        bl->next = hashTable[hashed];
        hashTable[hashed] = bl;
    } else{
        // Already in symbol table, push new line number to line number list
        LineNumberList ll = bl->lineNumbers;
        while(ll->next != NULL){
            ll = ll->next;
        }

        // TODO: check if line number already in list -> prevent duplicate line number entries 
        //   ex:   0005:  x := x * 1;  -> two references to line number 0005
        ll->next = (LineNumberList) malloc(sizeof(struct LineNumberListRecord));
        ll->next->lineno = lineno;
        ll->next->next = NULL;
    }
}


// Lookup symbol and return memory address, -1 if not found
int lookupSymbol(char *name){
    int hashed = hash(name);
    BucketList bl = hashTable[hashed];

    while((bl != NULL) && (strcmp(name, bl->name) != 0)){
        bl = bl->next;
    }
    if(bl == NULL){
        return -1;
    }
    return bl->memoryAddr;
}


// print formatted symbol table
// TODO: order by memory address ??
void printSymbolTable(FILE *f){
    fprintf(f, "Symbol         Address     Reference(s)\n");
    fprintf(f, "-----------    --------    ------------\n");
    for(int i = 0; i < PRIME; i++){
        if(hashTable[i] != NULL){
            BucketList bl = hashTable[i];
            while(bl != NULL){
                LineNumberList ll = bl->lineNumbers;
                fprintf(f, "%-14s ", bl->name);
                fprintf(f, "%-10d  ", bl->memoryAddr);
                while(ll != NULL){
                    fprintf(f, "%04d ", ll->lineno);
                    ll = ll->next;
                }
                fprintf(f, "\n");
                bl = bl->next;
            }
        }
    }
}

