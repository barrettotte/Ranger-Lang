#include "ranger-lang.h"


int gen_AST(AstNode_t *n, int reg){
    int leftReg, rightReg;

    if(n->left){
        leftReg = gen_AST(n->left, -1);
    }
    if(n->right){
        rightReg = gen_AST(n->right, leftReg);
    }
    switch(n->op){
        case A_ADD:         return cg_Add(leftReg, rightReg);
        case A_SUBTRACT:    return cg_Sub(leftReg, rightReg);
        case A_MULTIPLY:    return cg_Mul(leftReg, rightReg);
        case A_DIVIDE:      return cg_Div(leftReg, rightReg);
        case A_INTLITERAL:  return cg_LoadInt(n->v.intvalue);
        case A_IDENTIFIER:  return cg_LoadSymbol(g_Symtbl[n->v.id].name);
        case A_LVIDENT:     return cg_StoreSymbol(reg, g_Symtbl[n->v.id].name);
        case A_ASSIGN:      return rightReg;
    }
    fatald("Unknown AST operator %d\n", n->op);
    return -1;
}

void gen_Preamble(){
    cg_Preamble();
}

void gen_Postamble(){
    cg_Postamble();
}

void gen_ResetRegisters(){
    cg_ResetRegisters();
}

void gen_PrintInt(int reg){
    cg_PrintInt(reg);
}

void gen_NewSymbol(char *s){
    cg_NewSymbol(s);
}
