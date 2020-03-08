#include "common.h"
#include "globals.h"
#include "interface.h"


static void init(){
    g_line = 1;
    g_putback = '\n';
}

static void usage(char *pgm){
    fprintf(stderr, "Usage: %s <infile>\n", pgm);
    exit(1);
}

char *tokstr[] = {"+", "-", "*", "/", "intlit"};

static void lexFile(){
    struct token t;
    while(lex(&t)){
        printf("Token %s", tokstr[t.token]);
        if(t.token == T_INTLIT){
            printf(", value %d", t.intvalue);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        usage(argv[0]);
    }
    init();

    if((g_infile = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    lexFile();
    return 0;
}
