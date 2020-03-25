// Code generator interface for RangerVM

#ifndef GEN_RANGERVM_H
#define GEN_RANGERVM_H

/* Definitions */
#define AC  0   // accumulator
#define AC2 1   // another accumulator
#define GP  5   // global pointer - bottom of memory (globals)
#define MP  6   // memory pointer - top of memory (temps)
#define PC  7   // program counter


/* Function Prototypes */
void gen_InsReg(char *op, int t, int s1, int s2, char *c); // Generate 3-address asm, register to register
void gen_InsMem(char *op, int t, int off, int b, char *c); // Generate 2-address asm, register to memory
void gen_InsMemAbs(char *op, int r, int a, char *c);       // Generate 2-address asm with abs addr relative to pc
int  gen_Skip(int x);                                      // Skip addresses for future backpatch
void gen_Backtrack(int l);                                 // Backtracks to previous address
void gen_Restore();                                        // Restore current address to highest ungenerated address
void gen_Comment(char *c);                                 // Generate comment


#endif
