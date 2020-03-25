; RangerLang compilation to RangerVM Assembly
; File: /home/barrett/Programming/Repos/Ranger-Lang/tests/fibonacci.rg
;  
; START PRELUDE:
  0:     LDM   R6,0[R0] 		; load maxaddress from address 0
  1:     STM   R0,0[R0] 		; clear address 0
; END PRELUDE
  2:      IN   R0,R0,R0 		; read integer value
  3:     STM   R0,0[R5] 		; read: store value
  4:     LDI   R0,1[R0] 		; load constant
  5:     STM   R0,1[R5] 		; assign: store value
; IF
  6:     LDM   R0,0[R5] 		; load identifier value
  7:     STM   R0,0[R6] 		; operator: push left
  8:     LDI   R0,1[R0] 		; load constant
  9:     LDM   R1,0[R6] 		; operator: load left
 10:     SUB   R0,R1,R0 		; operator >
 11:     JGT   R0,2[R7] 		; branch if true
 12:     LDI   R0,0[R0] 		; it was false
 13:     LDA   R7,1[R7] 		; unconditioned jump
 14:     LDI   R0,1[R0] 		; it was true
 16:     LDI   R0,0[R0] 		; load constant
 17:     STM   R0,2[R5] 		; assign: store value
 18:     LDI   R0,1[R0] 		; load constant
 19:     STM   R0,3[R5] 		; assign: store value
 20:     LDI   R0,2[R0] 		; load constant
 21:     STM   R0,4[R5] 		; assign: store value
; DOW
 22:     LDM   R0,1[R5] 		; load identifier value
 23:     STM   R0,2[R5] 		; assign: store value
 24:     LDM   R0,1[R5] 		; load identifier value
 25:     STM   R0,0[R6] 		; operator: push left
 26:     LDM   R0,3[R5] 		; load identifier value
 27:     LDM   R1,0[R6] 		; operator: load left
 28:     ADD   R0,R1,R0 		; operator +
 29:     STM   R0,1[R5] 		; assign: store value
 30:     LDM   R0,2[R5] 		; load identifier value
 31:     STM   R0,3[R5] 		; assign: store value
 32:     LDM   R0,4[R5] 		; load identifier value
 33:     STM   R0,0[R6] 		; operator: push left
 34:     LDI   R0,1[R0] 		; load constant
 35:     LDM   R1,0[R6] 		; operator: load left
 36:     ADD   R0,R1,R0 		; operator +
 37:     STM   R0,4[R5] 		; assign: store value
 38:     LDM   R0,4[R5] 		; load identifier value
 39:     STM   R0,0[R6] 		; operator: push left
 40:     LDM   R0,0[R5] 		; load identifier value
 41:     STM   R0,-1[R6] 		; operator: push left
 42:     LDI   R0,1[R0] 		; load constant
 43:     LDM   R1,-1[R6] 		; operator: load left
 44:     SUB   R0,R1,R0 		; operator -
 45:     LDM   R1,0[R6] 		; operator: load left
 46:     SUB   R0,R0,R1 		; operator <
 47:     JLT   R0,2[R7] 		; branch if true
 48:     LDI   R0,0[R0] 		; it was false
 49:     LDA   R7,1[R7] 		; unconditioned jump
 50:     LDI   R0,1[R0] 		; it was true
 51:     JEQ   R0,-30[R7] 		; dow: jump back to start
; ENDDO
 15:     JEQ   R0,37[R7] 		; if: jump to else
 52:     LDA   R7,0[R7] 		; jump to end
; ENDIF
 53:     LDM   R0,1[R5] 		; load identifier value
 54:     OUT   R0,R0,R0 		; write AC
; END PROGRAM
 55:    HALT   R0,R0,R0 		; 
