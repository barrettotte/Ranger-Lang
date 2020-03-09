#include "ranger-lang.h"


// Generate code recursively, depth first traversal
int gen_AST(AstNode_t *n){
    int leftReg, rightReg;

    if(n->left){
        leftReg = gen_AST(n->left);
    }
    if(n->right){
        rightReg = gen_AST(n->right);
    }
    switch(n->op){
        case A_ADD:       return cgadd(leftReg, rightReg);
        case A_SUBTRACT:  return cgsub(leftReg, rightReg);
        case A_MULTIPLY:  return cgmul(leftReg, rightReg);
        case A_DIVIDE:    return cgdiv(leftReg, rightReg);
        case A_INTLIT:    return cgload(n->intvalue);
        default:
            fprintf(stderr, "Unknown AST operator %d\n", n->op);
            exit(1);
    }
}

void gen_Preamble(){
    cgpreamble();
}

void gen_Postamble(){
    cgpostamble();
}

void gen_Freeregs(){
    resetRegisters();
}

void gen_Printint(int reg){
    cgprintint(reg);
}
