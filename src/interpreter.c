#include "ranger-lang.h"

//static char *astOp[] = {"+","-","*","/"};


int interpretAst(AstNode *n){
    int leftval, rightval;

    if(n->left){
        leftval = interpretAst(n->left);
    }
    if(n->right){
        rightval = interpretAst(n->right);
    }

    /*if(n->op == A_INTLIT){
        printf("int %d\n", n->intvalue);
    } else {
        printf("%d %s %d\n", leftval, astOp[n->op], rightval);
    }*/

    switch(n->op){
        case A_ADD:         return leftval + rightval;
        case A_SUBTRACT:    return leftval - rightval;
        case A_DIVIDE:      return leftval / rightval;
        case A_MULTIPLY:    return leftval * rightval;
        case A_INTLIT:      return n->intvalue;
    }
    fprintf(stderr, "Uknown AST operator %d\n", n->op);
    exit(1);
}