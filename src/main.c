#include <linux/limits.h>

#include "rangerlang.h"


// Initialize global variables
void init(){
    g_Lineno = 0;
    g_Error = FALSE;
}


// Close file pointers and whatever else
void cleanup(){
    fclose(g_Listing);
    fclose(g_Target);
}


int main(int argc, char *argv[]){
    TreeNode *syntaxTree;
    char sourceName[64];
    char sourcePath[PATH_MAX];
    char listingPath[PATH_MAX];
    char targetPath[PATH_MAX];
    int sourcePathLen = 0;

    if(argc != 2){
        fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
        exit(1);
    }
    init();

    // Append file extension if none provided
    strcpy(sourceName, argv[1]);
    if(strchr(sourceName, '.') == NULL){
        strcat(sourceName, ".rl");
    }

    // Setup target and compile listing file paths
    realpath(sourceName, sourcePath);
    sourcePathLen = strlen(sourcePath);
    strncpy(listingPath, sourcePath, sourcePathLen-3);
    strcat(listingPath, "-listing.txt");
    strncpy(targetPath, sourcePath, sourcePathLen-3);
    strcat(targetPath, ".asm");
    
    g_Source = fopen(sourcePath, "r");
    if(g_Source == NULL){
        fprintf(stderr, "Could not find '%s'\n", sourcePath);
        return 1;
    }

    //g_Listing = stdout;
    g_Listing = fopen(listingPath, "w");
    if(g_Listing == NULL){
        printf("Could not open %s\n", listingPath);
        return 1;
    }
    fprintf(g_Listing, "\nRangerLang Compile Listing for %s\n", sourcePath);


    // Lexical and Syntax analysis
    syntaxTree = parse();
    fprintf(g_Listing, "\nSyntax Tree:\n");
    printTree(syntaxTree);

    // TODO: Build symbol table and semantic analysis

    // TODO: Code generation

    if(!g_Error){
        g_Target = fopen(targetPath, "w");
        if(g_Target == NULL){
            printf("Could not open %s\n", targetPath);
            return 1;
        }
        fprintf(g_Target, "; Compilation of %s\n", sourcePath);
    }

    cleanup();
    return 0;
}