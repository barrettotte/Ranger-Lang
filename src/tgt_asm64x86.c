#include "ranger-lang.h"

#define REG_LEN 4


// TODO: struct ? 
static int registers[REG_LEN];

static char *registerNames[REG_LEN] = {
    "%r8", "%r9", "%r10", "%r11"
};



// Reset state of all registers
void resetRegisters(){
    for(int i = 0; i < REG_LEN; i++){
        registers[i] = 1;
    }
}

// Reset state of register
static void resetRegister(int reg){
    if(registers[reg] != 0){
        fprintf(stderr, "Error trying to reset register %d\n", reg);
        exit(1);
    }
    registers[reg] = 1;
}

// Allocate new register, exit if none available
static int allocateRegister(){
    for(int i = 0; i < REG_LEN; i++){
        if(registers[i]){
            registers[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "Could not allocate new register, out of registers\n");
    exit(1);
}


void cgpreamble() {
    resetRegisters();
    fputs(
        "\t.text\n"
        ".LC0:\n"
        "\t.string\t\"%d\\n\"\n"
        "printint:\n"
        "\tpushq\t%rbp\n"
        "\tmovq\t%rsp, %rbp\n"
        "\tsubq\t$16, %rsp\n"
        "\tmovl\t%edi, -4(%rbp)\n"
        "\tmovl\t-4(%rbp), %eax\n"
        "\tmovl\t%eax, %esi\n"
        "\tleaq	.LC0(%rip), %rdi\n"
        "\tmovl	$0, %eax\n"
        "\tcall	printf@PLT\n"
        "\tnop\n"
        "\tleave\n"
        "\tret\n"
        "\n"
        "\t.globl\tmain\n"
        "\t.type\tmain, @function\n"
        "main:\n" "\tpushq\t%rbp\n" 
        "\tmovq	%rsp, %rbp\n", 
        g_Target
    );
}

void cgpostamble(){
    fputs("\tmovl	$0, %eax\n" "\tpopq	%rbp\n" "\tret\n", g_Target);
}


int cgload(int value){
    int r = allocateRegister();
    fprintf(g_Target, "\tmovq\t$%d, %s\n", value, registerNames[r]);
    return r;
}

int cgadd(int r1, int r2){
    fprintf(g_Target, "\taddq\t%s, %s\n", registerNames[r1], registerNames[r2]);
    resetRegister(r1);
    return r2;
}

int cgsub(int r1, int r2){
    fprintf(g_Target, "\tsubq\t%s, %s\n", registerNames[r2], registerNames[r1]);
    resetRegister(r2);
    return r1;
}

int cgmul(int r1, int r2){
    fprintf(g_Target, "\timulq\t%s, %s\n", registerNames[r1], registerNames[r2]);
    resetRegister(r1);
    return r2;
}

int cgdiv(int r1, int r2){
    fprintf(g_Target, "\tmovq\t%s,%%rax\n", registerNames[r1]);
    fprintf(g_Target, "\tcqo\n");
    fprintf(g_Target, "\tidivq\t%s\n", registerNames[r2]);
    fprintf(g_Target, "\tmovq\t%%rax,%s\n", registerNames[r1]);
    resetRegister(r2);
    return r1;
}

void cgprintint(int r){
    fprintf(g_Target, "\tmovq\t%s, %%rdi\n", registerNames[r]);
    fprintf(g_Target, "\tcall\tprintint\n");
    resetRegister(r);
}
