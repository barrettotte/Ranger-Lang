; List of opcodes for RangerVM
;
; 
; GP = R5 <-- global pointer - bottom of memory (globals)
; MP = R6 <-- memory pointer - top of memory (temps)
; PC = R7 <-- program counter
;
;
; Three address instructions 
; (register, register, register)
HALT  R0,R0,R0  ; Halt program (operands 1-3 ignored)
IN    R0,R0,R0  ; Read integer from STDIN to R0 (operands 2 and 3 ignored)
OUT   R0,R0,R0  ; Write integer in operand 1 to STDOUT (operands 2 and 3 ignored)
ADD   R0,R1,R2  ; Add operand 2 and operand 3, store in operand 1
SUB   R0,R1,R2  ; Subtract operand 2 and 3, store in operand 1
MUL   R0,R1,R2  ; Multiply operand 2 and 3, store in operand 1
DIV   R0,R1,R2  ; Divide operand 2 and 3, store in operand 1
;
;
; Two address instructions 
; (register, memory address)
LDM   R6,0[R0]  ; Load operand 1 with memory value at address ([operand 3] offset by operand 2)
LDA   R7,1[R7]  ; Load operand 1 with memory address of ([operand 3] offset by operand 2)
LDI   R0,1[R0]  ; Load operand 1 with immediate value operand 2 (operand 3 ignored)
STM   R0,1[R5]  ; Store value in operand 1 to memory address of ([operand 3] offset by operand 2)
JLT   R0,2[R7]  ; Jump to instruction at address of ([operand 3] offset by operand 2) if R0  < 0
JLE   R0,2[R7]  ; Jump to instruction at address of ([operand 3] offset by operand 2) if R0 <= 0
JGE   R0,2[R7]  ; Jump to instruction at address of ([operand 3] offset by operand 2) if R0 >= 0
JGT   R0,2[R7]  ; Jump to instruction at address of ([operand 3] offset by operand 2) if R0  > 0
JEQ   R0,2[R7]  ; Jump to instruction at address of ([operand 3] offset by operand 2) if R0 == 0
JNE   R0,2[R7]  ; Jump to instruction at address of ([operand 3] offset by operand 2) if R0 != 0

