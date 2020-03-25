; RangerLang compilation to RangerVM Assembly
; File: /home/barrett/Programming/Repos/Ranger-Lang/tests/test01.rg
;  
; START PRELUDE:
  0:     LDM   R6,0[R0] 		; load maxaddress from address 0
  1:     STM   R0,0[R0] 		; clear address 0
; END PRELUDE
  2:     LDI   R0,2[R0] 		; load constant
  3:     STM   R0,0[R5] 		; assign: store value
  4:     LDI   R0,1[R0] 		; load constant
  5:     STM   R0,0[R5] 		; assign: store value
  6:     LDM   R0,0[R5] 		; load identifier value
  7:     STM   R0,1[R5] 		; assign: store value
  8:     LDM   R0,1[R5] 		; load identifier value
  9:     STM   R0,0[R6] 		; operator: push left
 10:     LDI   R0,4[R0] 		; load constant
 11:     LDM   R1,0[R6] 		; operator: load left
 12:     ADD   R0,R1,R0 		; operator +
 13:     STM   R0,0[R5] 		; assign: store value
 14:     LDI   R0,6[R0] 		; load constant
 15:     STM   R0,0[R6] 		; operator: push left
 16:     LDI   R0,2[R0] 		; load constant
 17:     LDM   R1,0[R6] 		; operator: load left
 18:     DIV   R0,R1,R0 		; operator /
 19:     STM   R0,0[R5] 		; assign: store value
; END PROGRAM
 20:    HALT   R0,R0,R0 		; 
