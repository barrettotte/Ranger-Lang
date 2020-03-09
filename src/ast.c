#include "ranger-lang.h"


// Create a new AST node
AstNode *newAstNode(int op, AstNode *left, AstNode *right, int intvalue){
    AstNode *n;
    
    n = (AstNode *) malloc(sizeof(AstNode));
    if(n == NULL){
        fprintf(stderr, "Could not malloc new AST node in newAstNode()\n");
        exit(1);
    }
    n->op = op;
    n->left = left;
    n->right = right;
    n->intvalue = intvalue;    
    return n;
}


// Create a new AST leaf
AstNode *newAstLeaf(int op, int intvalue){
    return newAstNode(op, NULL, NULL, intvalue);
}


// Create a unary AST node
AstNode *newAstUnary(int op, AstNode *left, int intvalue){
    return newAstNode(op, left, NULL, intvalue);
}
