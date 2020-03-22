; RangerLang compilation to RangerVM Assembly
; File: /home/barrett/Programming/Repos/Ranger-Lang/tests/test01.rg
;  
; START PRELUDE:
  0:      LD   R6,0[R0] 		; load maxaddress from location 0
  1:      ST   R0,0[R0] 		; clear location 0
; END PRELUDE
  2:     LDC   R0,2[R0] 		; load constant
  3:      ST   R0,0[R5] 		; assign: store value
  4:     LDC   R0,1[R0] 		; load constant
  5:      ST   R0,0[R5] 		; assign: store value
  6:      LD   R0,0[R5] 		; load identifier value
  7:      ST   R0,1[R5] 		; assign: store value
  8:      LD   R0,1[R5] 		; load identifier value
  9:      ST   R0,0[R6] 		; operator: push left
 10:     LDC   R0,4[R0] 		; load constant
 11:      LD   R1,0[R6] 		; operator: load left
 12:     ADD   R0,R1,R0 		; operator +
 13:      ST   R0,0[R5] 		; assign: store value
 14:     LDC   R0,6[R0] 		; load constant
 15:      ST   R0,0[R6] 		; operator: push left
 16:     LDC   R0,2[R0] 		; load constant
 17:      LD   R1,0[R6] 		; operator: load left
 18:     DIV   R0,R1,R0 		; operator /
 19:      ST   R0,0[R5] 		; assign: store value
; END PROGRAM
 20:    HALT   R0,R0,R0 		; 
