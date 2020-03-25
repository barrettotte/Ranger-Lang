// Basic virtual machine designed to run compiled RangerLang source

#include <linux/limits.h>
#include "rangervm.h"


static int registers[REGS_SIZE];
static INSTRUCTION rom[ROM_SIZE];
static int ram[RAM_SIZE];


static char *opcodeTable[] = {
    "HALT", "IN", "OUT", "ADD", "SUB", "MUL", "DIV", "XXXX",         // REGISTER
    "LDM", "STM", "XXXX",                                            // MEMORY
    "LDA", "LDI", "JLT", "JLE", "JGT", "JGE", "JEQ", "JNE", "XXXX"   // ADDRESS
};
static char *pgmStateTable[] = {
    "OK", "PGM HALTED", 
    "ROM FAULT", "RAM FAULT", 
    "DIV BY ZERO"
};


// Util variables for reading in asm
static int currentNum;
static char currentWord[WORD_SIZE];
static char currentChar;
static char currentLine[LINE_SIZE];
static int currentCol;
static int lineLen;


// Debug util for dumping register contents
static void dumpRegisters(){
    for(int i = 0; i < REGS_SIZE; i++){
        printf("            R%d: %4d  ", i, registers[i]);
        if(i % 4 == 3) printf("\n");
    }
}


// Return op type for given opcode
static int getOpType(const int c){
    if(c <= OP_LIMIT_REG){
        return OPTYPE_REGISTER;
    } else if(c <= OP_LIMIT_MEM){
        return OPTYPE_MEMORY;
    }
    return OPTYPE_ADDRESS;
}


// Get next character in next column on current line
static void getNextChar(){
    if(++currentCol < lineLen){
        currentChar = currentLine[currentCol];
    } else{
        currentChar = ' ';
    }
}


// Print instruction to stdout
static void printInstruction(const int romAddr){
    printf("%5d: ", romAddr);
    if((romAddr >= 0) && (romAddr < ROM_SIZE)){
        OPTYPE optype = getOpType(rom[romAddr].opcode);
        printf("%6s R%d,", opcodeTable[rom[romAddr].opcode], rom[romAddr].arg1);

        if((optype == OPTYPE_ADDRESS) || (optype == OPTYPE_MEMORY)){
            printf("%d[R%d]\n", rom[romAddr].arg2, rom[romAddr].arg3);  
        } else if(optype == OPTYPE_REGISTER){
            printf("R%d,R%d\n", rom[romAddr].arg2, rom[romAddr].arg3);   
        } 
    }
}


// Get next non-whitespace character
static int getNextNonBlank(){
    while((currentCol < lineLen) && currentLine[currentCol] == ' '){
        currentCol++;
    }
    if(currentCol < lineLen){
        currentChar = currentLine[currentCol];
        return TRUE;
    }
    currentChar = ' ';
    return FALSE;
}


// Get next number
static int getNextNumber(){
    int sign = 0, term = 0, wasFetched = FALSE;
    currentNum = 0;
    do{
        sign = 1;
        while(getNextNonBlank() && ((currentChar == '+') || (currentChar == '-'))){
            wasFetched = FALSE;
            if(currentChar == '-'){
                sign = -sign;
                getNextChar();
            }
        }
        term = 0;
        getNextNonBlank();
        while(isdigit(currentChar)){
            wasFetched = TRUE;
            term = term * 10 + (currentChar - '0');
            getNextChar();
        }
        currentNum += (term * sign);
    } while(getNextNonBlank() && ((currentChar == '+') || (currentChar == '-')));
    return wasFetched;
}


// Get next word
static int getNextWord(){
    int len = 0;
    if(getNextNonBlank()){
        while(isalnum(currentChar)){
            if(len < WORD_SIZE-1){
                currentWord[len++] = currentChar;
            }
            getNextChar();
        }
        currentWord[len] = '\0';
        return (len != 0);
    }
    return FALSE;
}


// skip next character if its the specified character
static int skipChar(const char c){
    if(getNextNonBlank() && (currentChar == c)){
        getNextChar();
        return TRUE;
    }
    return FALSE;
}


// print when error occurs
static int error(const char *msg, const int lineno, const int instno){
    printf("Line %d", lineno);
    if(instno >= 0){
        printf(" (Instruction %d)", instno);
    }
    printf("    %s\n  %s\n", msg, currentLine);
    return FALSE;
}


// Initialize registers, RAM, and ROM
static void initVM(){
    for(int reg = 0; reg < REGS_SIZE; reg++){
        registers[reg] = 0;
    }
    ram[0] = RAM_SIZE-1;
    for(int addr = 1; addr < RAM_SIZE; addr++){
        ram[addr] = 0;
    }
    for(int addr = 0; addr < ROM_SIZE; addr++){
        rom[addr].opcode = OP_HALT;
        rom[addr].arg1 = 0;
        rom[addr].arg2 = 0;
        rom[addr].arg3 = 0;
    }
}


// Load instructions from file into ROM
static int loadPgm(FILE *pgm){
    int arg1=0, arg2=0, arg3=0, addr=0, lineno=0;
    OPCODE opcode = OP_HALT;
    
    initVM();
    while(!feof(pgm)){
        fgets(currentLine, LINE_SIZE-2, pgm);
        currentCol = 0;
        lineno++;
 
        lineLen = strlen(currentLine)-1;
        if(currentLine[lineLen] == '\n'){
            currentLine[lineLen] = '\0';
        } else{
            currentLine[++lineLen] = '\0';
        }
        if(getNextNonBlank() && (currentLine[currentCol] != ';')){
            if(!getNextNumber()){
                return error("Bad address", lineno, -1);
            }
            addr = currentNum;

            if(addr > ROM_SIZE){
                return error("Address out of bounds", lineno, addr); 
            } else if(!skipChar(':')){
                return error("Missing colon", lineno, addr);
            } else if(!getNextWord()){
                return error("Missing opcode", lineno, addr);
            }
            opcode = OP_HALT;

            while((opcode < OP_LIMIT_ADDR) && (strncmp(opcodeTable[opcode], currentWord, 4) != 0)){
                opcode++;
            }
            if(strncmp(opcodeTable[opcode], currentWord, 4) != 0){
                return error("Illegal opcode", lineno, addr);
            }

            // Ugly code inbound...this seemed the easiest to chew on while writing it
            // Load in instruction operands based on opcode type
            OPTYPE optype = getOpType(opcode);
            if(optype == OPTYPE_REGISTER){
                if((!skipChar('R')) || (!getNextNumber()) || (currentNum < 0) || (currentNum >= REGS_SIZE)){
                    return error("Invalid register in position 1", lineno, addr);
                }
                arg1 = currentNum;

                if(!skipChar(',')){
                    return error("Missing comma", lineno, addr);
                } else if((!skipChar('R')) || (!getNextNumber()) || (currentNum < 0) || (currentNum >= REGS_SIZE)){
                    return error("Invalid register in position 2", lineno, addr);
                }
                arg2 = currentNum;

                if(!skipChar(',')){
                    return error("Missing comma", lineno, addr);
                } else if((!skipChar('R')) || (!getNextNumber()) || (currentNum < 0) || (currentNum >= REGS_SIZE)){
                    return error("Invalid register in position 3", lineno, addr);
                }
                arg3 = currentNum;
            } else if((optype == OPTYPE_MEMORY) || (optype == OPTYPE_ADDRESS)){
                if((!skipChar('R')) || (!getNextNumber()) || (currentNum < 0) || (currentNum >= REGS_SIZE)){
                    return error("Invalid register in position 1", lineno, addr);
                }
                arg1 = currentNum;

                if(!skipChar(',')){
                    return error("Missing comma", lineno, addr);
                } else if(!getNextNumber()){
                    return error("Invalid memory offset", lineno, addr);
                }
                arg2 = currentNum;

                if((!skipChar('[')) && (!skipChar(','))){
                    return error("Missing left bracket", lineno, addr);
                } else if((!skipChar('R')) || (!getNextNumber()) || (currentNum < 0) || (currentNum >= REGS_SIZE)){
                    return error("Invalid register in position 3", lineno, addr);
                } else if((!skipChar(']'))){
                    return error("Missing right bracket", lineno, addr);
                }
                arg3 = currentNum;
            }
            rom[addr].opcode = opcode;
            rom[addr].arg1 = arg1;
            rom[addr].arg2 = arg2;
            rom[addr].arg3 = arg3;
        }
    }
    return TRUE;
}


// get input line from STDIN
static int getInput(char *msg, char *buff, size_t s){
    int ch=0, extra=0;

    if(msg != NULL){
        printf("%s", msg);
        fflush(stdout);
    }
    if(fgets(buff, s, stdin) == NULL){
        return FALSE;
    }
    if(buff[strlen(buff)-1] != '\n'){
        extra = 0;
        while(((ch = getchar()) != '\n') && (ch != EOF)){
            extra = 1;
        }
        return (extra == 1) ? FALSE : TRUE;
    }
    buff[strlen(buff)-1] = '\0';
    return TRUE;
}


// Step program one instruction
static PGMSTATE step(){
    INSTRUCTION instruction;
    int tgtReg=0, srcReg1=0, srcReg2=0, addr=0, inputValid=0;
    int pc = registers[REG_PC];

    if((pc < 0) || (pc > ROM_SIZE)){
        return STATE_ROM_ERR;
    }
    registers[REG_PC] = pc + 1;
    if(DEBUG_VM){
        dumpRegisters();
    }
    instruction = rom[pc];

    // set operands depending on instruction type
    OPTYPE opType = getOpType(instruction.opcode);
    tgtReg = instruction.arg1;
    if(opType == OPTYPE_REGISTER){
        srcReg1 = instruction.arg2;
        srcReg2 = instruction.arg3;
    } else{
        srcReg1 = instruction.arg3;
        if((opType == OPTYPE_MEMORY) && ((addr < 0) || (addr > RAM_SIZE))){
            return STATE_RAM_ERR;
        }
        addr = instruction.arg2 + registers[srcReg1];
    }
    switch(instruction.opcode){
        case OP_HALT:  return STATE_HALT;
        case OP_LDM:   registers[tgtReg] = ram[addr];                                break;
        case OP_STM:   ram[addr] = registers[tgtReg];                                break;
        case OP_LDA:   registers[tgtReg] = addr;                                     break;
        case OP_LDI:   registers[tgtReg] = instruction.arg2;                         break;
        case OP_JEQ:   if(registers[tgtReg] == 0) registers[REG_PC] = addr;          break;
        case OP_JNE:   if(registers[tgtReg] != 0) registers[REG_PC] = addr;          break;
        case OP_JLT:   if(registers[tgtReg] <  0) registers[REG_PC] = addr;          break;
        case OP_JGT:   if(registers[tgtReg] >  0) registers[REG_PC] = addr;          break;
        case OP_JLE:   if(registers[tgtReg] <= 0) registers[REG_PC] = addr;          break;
        case OP_JGE:   if(registers[tgtReg] >= 0) registers[REG_PC] = addr;          break;
        case OP_OUT:   printf("OUT: %d\n", registers[tgtReg]);                       break;
        case OP_ADD:   registers[tgtReg] = registers[srcReg1] + registers[srcReg2];  break;
        case OP_SUB:   registers[tgtReg] = registers[srcReg1] - registers[srcReg2];  break;
        case OP_MUL:   registers[tgtReg] = registers[srcReg1] * registers[srcReg2];  break;
        case OP_DIV: 
            if(registers[srcReg2] == 0){
                return STATE_DIVBYZERO;
            }
            registers[tgtReg] = registers[srcReg1] / registers[srcReg2];  
            break;
        case OP_IN:
            do{
                getInput("IN:  ", currentLine, 8);
                lineLen = strlen(currentLine);
                currentCol = 0;
                inputValid = getNextNumber();
                if(!inputValid){
                    printf("Invalid input. Must be an integer.\n");
                } else{
                    registers[tgtReg] = currentNum;
                }
            } while(!inputValid);
            break;
    }
    return STATE_OKAY;
}


// Run program from ROM
static int runPgm(){
    int steps = 0, pgmState = STATE_OKAY, romAddr = 0;
    while(pgmState == STATE_OKAY){
        romAddr = registers[REG_PC];
        if(DEBUG_VM){
            printInstruction(romAddr);
        }
        pgmState = step();
        steps++;
    }
    printf("Executed %d instruction(s)\n", steps);
    printf("%s\n", pgmStateTable[pgmState]);
    return TRUE;
}


int main(int argc, char *argv[]){
    char pgmName[64], pgmPath[PATH_MAX];
    FILE *pgm;

    if(argc != 2){
        printf("usage: %s <filename>\n", argv[0]);
    }
    strcpy(pgmName, argv[1]);
    if(strchr(pgmName, '.') == NULL){
        strcat(pgmName, ".asm");
    }
    pgm = fopen(pgmName, "r");
    if(pgm == NULL){
        printf("file '%s' could not be opened\n", pgmName);
        exit(1);
    }
    realpath(pgmName, pgmPath);

    printf("RangerVM Started.\nLoading %s into ROM\n", pgmPath);
    if(!loadPgm(pgm)){
        printf("Error occurred running program.");
        exit(1);
    }
    runPgm();
    printf("RangerVM Ended.");
    return 0;
}
