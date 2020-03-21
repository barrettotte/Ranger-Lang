// General utilities

#include "rangerlang.h"

static int indentno = 0; // used by printTree

// Indentation macros
#define INDENT   indentno += 2
#define UNINDENT indentno -= 2


// fprintf to file only if cond is true
void fprintf_if(FILE *file, const char *msg, const int cond){
    if(cond){
        fprintf(file, "%s", msg);
    }
}


// Print file contents to STDOUT
void printFile(const char *filePath){
    FILE *file;
    int c;
    file = fopen(filePath, "r");
    if(file){
        while((c = getc(file)) != EOF){
            putchar(c);
        }
        fclose(g_Listing);
    } else{
        printf("Could not open %s\n", filePath);
    }
}


// create new statement node
TreeNode *newStmtNode(StmtKind stmtKind){
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));

    if(t != NULL){
        for(int i = 0; i < MAXTREECHILD; i++){
            t->children[i] = NULL;
            t->sibling = NULL;
            t->nodeKind = NK_Stmt;
            t->kind.stmt = stmtKind;
            t->lineno = g_Lineno;
        }
    } else{
        fprintf(g_Listing, "Out of memory error allocating new statement node; line %d\n", g_Lineno);
    }
    return t;
}


// create new expression node
TreeNode *newExprNode(ExprKind exprKind){
    TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));

    if(t != NULL){
        for(int i = 0; i < MAXTREECHILD; i++){
            t->children[i] = NULL;
            t->sibling = NULL;
            t->nodeKind = NK_Expr;
            t->kind.expr = exprKind;
            t->lineno = g_Lineno;
            t->type = ET_VOID;
        }
    } else {
        fprintf(g_Listing, "Out of memory error allocating new expression node; line %d\n", g_Lineno);
    }
    return t;
}


// print token and lexeme to compile listing 
void printToken(TokenType tokenType, const char *lexeme){
    switch(tokenType){
        case T_ASSIGN:     fprintf(g_Listing, ":=\n");  break;
        case T_LT:         fprintf(g_Listing, "<\n");   break;
        case T_GT:         fprintf(g_Listing, ">\n");   break;
        case T_EQ:         fprintf(g_Listing, "=\n");   break;
        case T_LPAREN:     fprintf(g_Listing, "(\n");   break;
        case T_RPAREN:     fprintf(g_Listing, ")\n");   break;
        case T_SEMICOLON:  fprintf(g_Listing, ";\n");   break;
        case T_PLUS:       fprintf(g_Listing, "+\n");   break;
        case T_MINUS:      fprintf(g_Listing, "-\n");   break;
        case T_STAR:       fprintf(g_Listing, "*\n");   break;
        case T_SLASH:      fprintf(g_Listing, "/\n");   break;
        case T_ENDFILE:    fprintf(g_Listing, "EOF\n"); break;
        case T_NUMBER:     fprintf(g_Listing, "literal, val = %s\n", lexeme);     break;
        case T_IDENTIFIER: fprintf(g_Listing, "identifier, name = %s\n", lexeme); break;
        case T_ERROR:      fprintf(g_Listing, "error: %s\n", lexeme);             break;
        case T_IF:
        case T_ELSE:
        case T_ENDIF:
        case T_DOW:
        case T_ENDDO:
        case T_READ:
        case T_WRITE:
            fprintf(g_Listing, "keyword: %s\n", lexeme);
            break;
        default: 
            fprintf(g_Listing, "?????? Unknown token: %d\n", tokenType); 
            g_Error = TRUE;
            break;
    }
}


// print spaces to compile listing according to indentation level
static void printSpaces(){
    for(int i = 0; i < indentno; i++){
        fprintf(g_Listing, " ");
    }
}


// print statement node to compile listing
static void printStmtNode(TreeNode *node){
    switch(node->kind.stmt){
        case SK_ASSIGN: fprintf(g_Listing, "Assignment: %s\n", node->attr.name); break;
        case SK_READ:   fprintf(g_Listing, "Read: %s\n", node->attr.name);       break;
        case SK_WRITE:  fprintf(g_Listing, "Write\n");                           break;
        case SK_IF:     fprintf(g_Listing, "If\n");                              break;
        case SK_DOW:    fprintf(g_Listing, "Dow\n");                             break;
        default:        fprintf(g_Listing, "Unknown Expression Node Kind\n");    break;
    }
}


// print expression node to compile listing
static void printExprNode(TreeNode *node){
    switch(node->kind.expr){
        case EK_OP:
            fprintf(g_Listing, "Operator: ");
            printToken(node->attr.op, "\0");
            break;
        case EK_LITERAL:      
            fprintf(g_Listing, "Literal: %d\n", node->attr.val);
            break;
        case EK_IDENTIFIER: 
            fprintf(g_Listing, "Identifier: %s\n", node->attr.name); 
            break;
        default:
            fprintf(g_Listing, "Unknown Expression Node Kind\n");
            break;
    }
}

// TODO: better tree printing, doesn't look good with nested blocks...
// print tree to compile listing
void printTree(TreeNode *tree){
    INDENT;
    while(tree != NULL){
        printSpaces();
        //fprintf(g_Listing, "Lineno: %d  ", tree->lineno); // somewhat useful debug print
        switch(tree->nodeKind){
            case NK_Stmt:  printStmtNode(tree);  break;
            case NK_Expr:  printExprNode(tree);  break;
            default:      
                fprintf(g_Listing, "Unknown expression node kind.\n"); 
                break;
        }
        for(int i = 0; i< MAXTREECHILD; i++){
            printTree(tree->children[i]);
        }
        tree = tree->sibling;
    }
    UNINDENT;
}


// make new copy of string s
char *copyString(char *s){
    char *t;

    if(s == NULL){
        return NULL;
    }
    int n = strlen(s) + 1;
    t = malloc(n);

    if(t == NULL){
        fprintf(g_Listing, "Out of memory error copying string; line %d\n", g_Lineno);
    } else{
        strcpy(t, s);
    }
    return t;
}