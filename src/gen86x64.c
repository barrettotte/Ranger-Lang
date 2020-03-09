#include "ranger-lang.h"

#define REG_SIZE 4

// List of available registers
static int registers[REG_SIZE];

static char *registerNames[REG_SIZE] = {
    "%r8", "%r9", "%r10", "%r11"
};

// TODO: move registers to struct ?


// Reset all register states to free
void cgasm_Reset(){
    for(int i = 0; i < REG_SIZE; i++){
        registers[i] = 1;
    }
}

// Allocate a free register, return register number
static int allocateRegister(){
    for(int i = 0; i < REG_SIZE; i++){
        if(registers[i]){
            registers[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "Out of registers...%d in use\n", REG_SIZE);
    exit(1);
}

// Free a register
static void freeRegister(int reg){
    if(registers[reg] != 0){
        fprintf(stderr, "Error occurred freeing register %d\n", reg);
        exit(1);
    }
    registers[reg] = 1;
}

// Generate assembly preamble
void cgasm_Preamble(){
    cgasm_Reset();
    fputs("", g_Target);
}

// Generate assembly postamble
void cgasm_Postamble(){
    fputs("", g_Target);
}

// Load integer literal into register, return register number
int cgasm_Load(int val){
    int r = allocateRegister();
    fprintf(g_Target, "\tmovq\t$%d, %s\n", val, registerNames[r]);
    return r;
}

// Add two registers and return result register number
int cgasm_Add(int r1, int r2){
    fprintf(g_Target, "\taddq\t%s, %s\n", registerNames[r1], registerNames[r2]);
    freeRegister(r1);
    return r2;
}

// Subtract two registers and return result register number
int cgasm_Sub(int r1, int r2){
    fprintf(g_Target, "\tsubq\t%s, %s\n", registerNames[r2], registerNames[r1]);
    freeRegister(r1);
    return r2;
}

// Multiply two registers and return result register number
int cgasm_Mul(int r1, int r2){
    fprintf(g_Target, "\timulq\t%s, %s\n", registerNames[r1], registerNames[r2]);
    freeRegister(r1);
    return r2;
}

// Divide two registers and return result register number
int cgasm_Div(int r1, int r2){
    fprintf(g_Target, "\tmovq\t%s, %%rax\n", registerNames[r1]);
    fprintf(g_Target, "\tcqo\n");
    fprintf(g_Target, "\tidivq\t%s\n", registerNames[r2]);
    fprintf(g_Target, "\tmovq\t%%rax, %s\n", registerNames[r1]);
    freeRegister(r2);
    return r1;
}

//
void cgasm_Printint(int r){
    fprintf(g_Target, "\tmovq\t%s, %%rdi\n", registerNames[r]);
    fprintf(g_Target, "\tcall\tprintint\n");
    freeRegister(r);
}