// Driver program for compiler

#include <linux/limits.h>
#include "rangerlang.h"


// Initialize global variables
void init(){
    g_Lineno = 0;
    g_Error = FALSE;
}

// Close file pointers and whatever else
void cleanup(){
    fclose(g_Source);
    if(!PRINT_STDOUT){
        fclose(g_Listing);
    }
    fclose(g_Target);
}

// Validate args, show usage and exit if not valid
void validateArgs(char argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
        exit(1);
    }
}

// Build file name based on source file path
char *buildFileName(const char *srcPath, const char *ext){
    char *s = malloc(sizeof (char) *PATH_MAX);
    int pathLen = strlen(srcPath);
    strncpy(s, srcPath, pathLen-3); // .rg (-3)
    strcat(s, ext);
    return s;
}

// If file open fails, program ends with error
void criticalOpen(FILE **file, const char *path, const char *mode){
    if((*file = fopen(path, mode)) == NULL){
        fprintf(stderr, "Could not open '%s'\n", path);
        exit(1);
    }
}

// Main compile function
void compile(const char *srcPath){
    TreeNode *syntaxTree;
    
    criticalOpen(&g_Source, srcPath, "r");

    if(PRINT_STDOUT){
        g_Listing = stdout;   
    } else{
        criticalOpen(&g_Listing, buildFileName(srcPath, "-listing.txt"), "w");
    }
    fprintf(g_Listing, "\nRangerLang Compile Listing for \n    %s\n", srcPath);

    // Lexical and Syntax analysis
    fprintf(g_Listing, "\nSource Program:\n");
    syntaxTree = parse();
    if(DEBUG_PARSER){
        fprintf(g_Listing, "\nSyntax Tree:\n");
        printTree(syntaxTree);
    }

    // Semantic Analysis
    fprintf(g_Listing, "\nBuilding symbol table...\n");
    buildSymbolTable(syntaxTree);
    fprintf(g_Listing, "\nChecking types...\n");
    checkType(syntaxTree);
    
    if(!g_Error){
        criticalOpen(&g_Target, buildFileName(srcPath, ".asm"), "w");
        fprintf(g_Listing, "\nGenerating code...\n");
        generateCode(syntaxTree, srcPath);
    }
    fprintf(g_Listing, "\nDONE.\n");
}


int main(int argc, char *argv[]){
    char srcName[64], srcPath[PATH_MAX];
    validateArgs(argc, argv);
    init();

    // Get source file full path with extension
    strcpy(srcName, argv[1]);
    if(strchr(srcName, '.') == NULL){
        strcat(srcName, ".rg"); // Append file extension if not given
    }
    realpath(srcName, srcPath);
    
    compile(srcPath);
    cleanup();
    
    return 0;
}