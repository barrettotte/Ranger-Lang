// Code generator interface for RangerVM

#include "rangerlang.h"
#include "gen_rangervm.h"


// Current instruction generation address
static int currAddr = 0;

// Current highest VM address generated
static int highestAddr = 0;


// Util for adjust highest address
static void adjustHighest(){
    if(highestAddr < currAddr){
        highestAddr = currAddr;
    }
}


// Generate a comment
void gen_Comment(char *c){
    fprintf(g_Target, "; %s\n", c);
}


// Generate three address asm for register to register instructions
void gen_InsReg(char *op, int tgt, int src1, int src2, char *comment){
    fprintf(g_Target, "%3d:   %5s   R%d,R%d,R%d \t\t; %s\n", currAddr++, op, tgt, src1, src2, comment);
    adjustHighest();
}


// Generate two address asm for register to memory instructions
void gen_InsMem(char *op, int tgt, int offset, int base, char *c){
    fprintf(g_Target, "%3d:   %5s   R%d,%d[R%d] \t\t; %s\n", currAddr++, op, tgt, offset, base, c);
    adjustHighest();
}


// Skip addresses for future backpatch
int gen_Skip(int x){
    int i = currAddr;
    currAddr += x;
    adjustHighest();
    return i;
}


// Backtracks to previous address
void gen_Backtrack(int l){
    if(l > highestAddr){
        gen_Comment("ERROR found in gen_Backtrack()");
    }
    currAddr = l;
}                               


// Restore current position to highest ungenerated position
void gen_Restore(){
    currAddr = highestAddr;
}


// Generate two addr asm with abs addr relative to pc
void gen_InsMemAbs(char *op, int tgt, int abs, char *comment){
    fprintf(g_Target, "%3d:   %5s   R%d,%d[R%d] \t\t; %s\n", currAddr, op, tgt, abs-(currAddr+1), PC, comment);
    ++currAddr;
    adjustHighest();
}
