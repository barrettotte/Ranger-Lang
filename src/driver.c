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
        fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }
    if((g_Target = fopen("out.s", "w")) == NULL){
        fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
        exit(1);
    }

    lex(&g_Token);
    gen_Preamble();
    parse_Statements();
    gen_Postamble();

    fclose(g_Target);
    return 0;
}