; RangerLang compilation to RangerVM Assembly
; File: /home/barrett/Programming/Repos/Ranger-Lang/tests/factorial.rg
;  
; START PRELUDE:
  0:      LD   R6,0[R0] 		; load maxaddress from location 0
  1:      ST   R0,0[R0] 		; clear location 0
; END PRELUDE
  2:      IN   R0,R0,R0 		; read integer value
  3:      ST   R0,0[R5] 		; read: store value
; IF
  4:      LD   R0,0[R5] 		; load identifier value
  5:      ST   R0,0[R6] 		; operator: push left
  6:     LDC   R0,0[R0] 		; load constant
  7:      LD   R1,0[R6] 		; operator: load left
  8:     SUB   R0,R1,R0 		; operator >
  9:     JGT   R0,2[R7] 		; branch if true
 10:     LDC   R0,0[R0] 		; it was false
 11:     LDA   R7,1[R7] 		; unconditioned jump
 12:     LDC   R0,1[R0] 		; it was true
 14:     LDC   R0,1[R0] 		; load constant
 15:      ST   R0,1[R5] 		; assign: store value
; DOW
 16:      LD   R0,0[R5] 		; load identifier value
 17:      ST   R0,0[R6] 		; operator: push left
 18:     LDC   R0,0[R0] 		; load constant
 19:      LD   R1,0[R6] 		; operator: load left
 20:     SUB   R0,R1,R0 		; operator ==
 21:     JEQ   R0,2[R7] 		; branch if true
 22:     LDC   R0,0[R0] 		; it was false
 23:     LDA   R7,1[R7] 		; unconditioned jump
 24:     LDC   R0,1[R0] 		; it was true
 25:      LD   R0,1[R5] 		; load identifier value
 26:      ST   R0,0[R6] 		; operator: push left
 27:      LD   R0,0[R5] 		; load identifier value
 28:      LD   R1,0[R6] 		; operator: load left
 29:     MUL   R0,R1,R0 		; operator *
 30:      ST   R0,1[R5] 		; assign: store value
 31:      LD   R0,0[R5] 		; load identifier value
 32:      ST   R0,0[R6] 		; operator: push left
 33:     LDC   R0,1[R0] 		; load constant
 34:      LD   R1,0[R6] 		; operator: load left
 35:     SUB   R0,R1,R0 		; operator -
 36:      ST   R0,0[R5] 		; assign: store value
 37:     JEQ   R0,-22[7] 		; dow: jump back to start
; ENDDO
 38:      LD   R0,1[R5] 		; load identifier value
 39:     OUT   R0,R0,R0 		; write AC
 13:     JEQ   R0,26[7] 		; if: jump to else
  0:     LDA   R7,39[7] 		; jump to end
; ENDIF
; END PROGRAM
 40:    HALT   R0,R0,R0 		; 
