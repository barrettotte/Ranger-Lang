; RangerLang compilation to RangerVM Assembly
; File: /home/barrett/Programming/Repos/Ranger-Lang/tests/fibonacci.rg
;  
; START PRELUDE:
  0:      LD   R6,0[R0] 		; load maxaddress from location 0
  1:      ST   R0,0[R0] 		; clear location 0
; END PRELUDE
  2:     LDC   R0,1[R0] 		; load constant
  3:      ST   R0,0[R5] 		; assign: store value
  4:      IN   R0,R0,R0 		; read integer value
  5:      ST   R0,1[R5] 		; read: store value
; IF
  6:      LD   R0,1[R5] 		; load identifier value
  7:      ST   R0,0[R6] 		; operator: push left
  8:     LDC   R0,1[R0] 		; load constant
  9:      LD   R1,0[R6] 		; operator: load left
 10:     SUB   R0,R1,R0 		; operator >
 11:     JGT   R0,2[R7] 		; branch if true
 12:     LDC   R0,0[R0] 		; it was false
 13:     LDA   R7,1[R7] 		; unconditioned jump
 14:     LDC   R0,1[R0] 		; it was true
 16:     LDC   R0,1[R0] 		; load constant
 17:      ST   R0,2[R5] 		; assign: store value
 18:     LDC   R0,2[R0] 		; load constant
 19:      ST   R0,3[R5] 		; assign: store value
; DOW
 20:      LD   R0,3[R5] 		; load identifier value
 21:      ST   R0,0[R6] 		; operator: push left
 22:      LD   R0,1[R5] 		; load identifier value
 23:      LD   R1,0[R6] 		; operator: load left
 24:     SUB   R0,R1,R0 		; operator <
 25:     JLT   R0,2[R7] 		; branch if true
 26:     LDC   R0,0[R0] 		; it was false
 27:     LDA   R7,1[R7] 		; unconditioned jump
 28:     LDC   R0,1[R0] 		; it was true
 29:      LD   R0,0[R5] 		; load identifier value
 30:      ST   R0,4[R5] 		; assign: store value
 31:      LD   R0,0[R5] 		; load identifier value
 32:      ST   R0,0[R6] 		; operator: push left
 33:      LD   R0,2[R5] 		; load identifier value
 34:      LD   R1,0[R6] 		; operator: load left
 35:     ADD   R0,R1,R0 		; operator +
 36:      ST   R0,0[R5] 		; assign: store value
 37:      LD   R0,4[R5] 		; load identifier value
 38:      ST   R0,2[R5] 		; assign: store value
 39:      LD   R0,3[R5] 		; load identifier value
 40:      ST   R0,0[R6] 		; operator: push left
 41:     LDC   R0,1[R0] 		; load constant
 42:      LD   R1,0[R6] 		; operator: load left
 43:     ADD   R0,R1,R0 		; operator +
 44:      ST   R0,3[R5] 		; assign: store value
 45:     JEQ   R0,-26[7] 		; dow: jump back to start
; ENDDO
 15:     JEQ   R0,30[7] 		; if: jump to else
 46:      LD   R0,1[R5] 		; load identifier value
 47:      ST   R0,0[R5] 		; assign: store value
  0:     LDA   R7,47[7] 		; jump to end
; ENDIF
 48:      LD   R0,0[R5] 		; load identifier value
 49:     OUT   R0,R0,R0 		; write AC
; END PROGRAM
 50:    HALT   R0,R0,R0 		; 
