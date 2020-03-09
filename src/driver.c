#include <errno.h>
#include "ranger-lang.h"


static void init(){
    g_Line = 1;
    g_Reinsert = '\n';
}

static void usage(char *prog) {
    fprintf(stderr, "Usage: %s <infile>\n", prog);
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        usage(argv[0]);
    }
    init();

    if((g_Source = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "Could not open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    if((g_Target = fopen("out.s", "w")) == NULL){
        fprintf(stderr, "Could not create out.s: %s\n", strerror(errno));
        return 1;
    }

    lex(&g_Token);
    gen_Preamble();
    parse_Statements();
    gen_Postamble();
    
    fclose(g_Target);
    return 0;
}