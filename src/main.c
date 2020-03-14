#include <linux/limits.h>
#include <stdlib.h>

#include "rangerlang.h"



// Initialize global variables
void init(){
    g_Lineno = 0;
    g_Error = FALSE;
}

int main(int argc, char *argv[]){
    TreeNode *syntaxTree;
    char filePath[PATH_MAX];
    char srcName[64];

    if(argc != 2){
        fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
        exit(1);
    }
    init();

    // Append file extension if none provided
    strcpy(srcName, argv[1]);
    if(strchr(srcName, '.') == NULL){
        strcat(srcName, ".rl");
    }
    realpath(srcName, filePath);
    
    g_Source = fopen(srcName, "r");
    if(g_Source == NULL){
        fprintf(stderr, "Could not find '%s'\n", filePath);
        exit(1);
    }

    //g_Listing = stdout;
    g_Listing = fopen("../out/listing.txt", "w");
    fprintf(g_Listing, "\nRangerLang Compile Listing for %s\n", srcName);


    // Lexical and Syntax analysis
    syntaxTree = parse();
    fprintf(g_Listing, "\nSyntax Tree:\n");
    printTree(syntaxTree);

    // TODO: Build symbol table and semantic analysis

    // TODO: Code generation

    if(!g_Error){
        g_Target = fopen("../out/test.asm", "w");
        if(g_Target == NULL){
            printf("Could not open %s\n", "../out/test.asm");
            exit(1);
        }

        fprintf(g_Target, "; Compilation of %s\n", filePath);
    }

    fclose(g_Listing);
    fclose(g_Target);
    return 0;
}