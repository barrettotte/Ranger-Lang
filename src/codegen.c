#include "ranger-lang.h"





// Generate assembly code recursively, depth first traversal
int gen_Ast(AstNode *n){
    int leftReg, rightReg;

    if(n->left){
        leftReg = gen_Ast(n->left);
    }
    if(n->right){
        rightReg = gen_Ast(n->right);
    }
    switch(n->op){
        case A_ADD:       return cgasm_Add(leftReg, rightReg);
        case A_SUBTRACT:  return cgasm_Sub(leftReg, rightReg);
        case A_MULTIPLY:  return cgasm_Mul(leftReg, rightReg);
        case A_DIVIDE:    return cgasm_Div(leftReg, rightReg);
        case A_INTLIT:    return cgasm_Load(n->intvalue);
        default:          break;
    }
    fprintf(stderr, "Unknown AST operator %d on line %d", n->op, g_Line);
    exit(1);
}

void gen_Preamble(){
    cgasm_Preamble();
}

void gen_Postamble(){
    cgasm_Postamble();
}

void gen_Reset(){
    cgasm_Reset();
}

void gen_Printint(int r){
    cgasm_Printint(r);
}
