// Basic virtual machine designed to run compiled RangerLang source

#ifndef RANGERVM_H
#define RANGERVM_H


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#define DEBUG_VM  FALSE

#define ROM_SIZE  1024
#define RAM_SIZE  1024
#define REGS_SIZE    8
#define REG_PC       7
#define LINE_SIZE  121
#define WORD_SIZE   20


typedef enum{
    OPTYPE_REGISTER, OPTYPE_MEMORY, OPTYPE_ADDRESS
} OPTYPE;

typedef enum{
    OP_HALT, OP_IN, OP_OUT, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_LIMIT_REG,
    OP_LDM, OP_STM, OP_LIMIT_MEM,
    OP_LDA, OP_LDI, OP_JLT, OP_JLE, OP_JGT, OP_JGE, OP_JEQ, OP_JNE, OP_LIMIT_ADDR
} OPCODE;

typedef enum{
    STATE_OKAY, STATE_HALT,
    STATE_ROM_ERR, STATE_RAM_ERR,
    STATE_DIVBYZERO
} PGMSTATE;


typedef struct{
    int opcode;
    int arg1;
    int arg2;
    int arg3;
} INSTRUCTION;


#endif
