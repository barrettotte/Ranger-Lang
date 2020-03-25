; RangerLang compilation to RangerVM Assembly
; File: /home/barrett/Programming/Repos/Ranger-Lang/tests/factorial.rg
;  
; START PRELUDE:
  0:     LDM   R6,0[R0] 		; load maxaddress from address 0
  1:     STM   R0,0[R0] 		; clear address 0
; END PRELUDE
  2:      IN   R0,R0,R0 		; read integer value
  3:     STM   R0,0[R5] 		; read: store value
; IF
  4:     LDM   R0,0[R5] 		; load identifier value
  5:     STM   R0,0[R6] 		; operator: push left
  6:     LDI   R0,0[R0] 		; load constant
  7:     LDM   R1,0[R6] 		; operator: load left
  8:     SUB   R0,R1,R0 		; operator >
  9:     JGT   R0,2[R7] 		; branch if true
 10:     LDI   R0,0[R0] 		; it was false
 11:     LDA   R7,1[R7] 		; unconditioned jump
 12:     LDI   R0,1[R0] 		; it was true
 14:     LDI   R0,1[R0] 		; load constant
 15:     STM   R0,1[R5] 		; assign: store value
; DOW
 16:     LDM   R0,1[R5] 		; load identifier value
 17:     STM   R0,0[R6] 		; operator: push left
 18:     LDM   R0,0[R5] 		; load identifier value
 19:     LDM   R1,0[R6] 		; operator: load left
 20:     MUL   R0,R1,R0 		; operator *
 21:     STM   R0,1[R5] 		; assign: store value
 22:     LDM   R0,0[R5] 		; load identifier value
 23:     STM   R0,0[R6] 		; operator: push left
 24:     LDI   R0,1[R0] 		; load constant
 25:     LDM   R1,0[R6] 		; operator: load left
 26:     SUB   R0,R1,R0 		; operator -
 27:     STM   R0,0[R5] 		; assign: store value
 28:     LDM   R0,0[R5] 		; load identifier value
 29:     STM   R0,0[R6] 		; operator: push left
 30:     LDI   R0,0[R0] 		; load constant
 31:     LDM   R1,0[R6] 		; operator: load left
 32:     SUB   R0,R1,R0 		; operator ==
 33:     JEQ   R0,2[R7] 		; branch if true
 34:     LDI   R0,0[R0] 		; it was false
 35:     LDA   R7,1[R7] 		; unconditioned jump
 36:     LDI   R0,1[R0] 		; it was true
 37:     JEQ   R0,-22[R7] 		; dow: jump back to start
; ENDDO
 38:     LDM   R0,1[R5] 		; load identifier value
 39:     OUT   R0,R0,R0 		; write AC
 13:     JEQ   R0,27[R7] 		; if: jump to else
 40:     LDA   R7,0[R7] 		; jump to end
; ENDIF
; END PROGRAM
 41:    HALT   R0,R0,R0 		; 
