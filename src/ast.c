#include "ranger-lang.h"


// Create a new AST node
AstNode_t *newAstNode(int op, AstNode_t *left, AstNode_t *right, int intvalue){
    AstNode_t *n;

    n = (AstNode_t *) malloc(sizeof(AstNode_t));
    if(n == NULL){
        fprintf(stderr, "Could not create new AST Node\n");
        exit(1);
    }
    n->op = op;
    n->left = left;
    n->right = right;
    n->intvalue = intvalue;
    return n;
}

// Create a new AST leaf
AstNode_t *newAstLeaf(int op, int intvalue){
    return newAstNode(op, NULL, NULL, intvalue);
}

// Create a new unary AST node
AstNode_t *newAstUnary(int op, AstNode_t *left, int intvalue){
  return newAstNode(op, left, NULL, intvalue);
}
