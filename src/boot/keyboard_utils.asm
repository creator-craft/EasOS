;*********************************************
;  vesa_utils.asm
;
;  Utility functions for VESA (Video Electronics Standard Association).
;*********************************************
[bits  16]

; **********
; Wait until user press any key
; out: ah (scancode), al (ascii character)
; - ax, si
; **********
wait_any_key:
  mov si, .PRESS_ANY_KEY_MSG
  call print
.key_loop:
  mov ah, 0x01
  int 16h
  jz .key_loop        ; wait until key press

  mov ax, 0x0E00 + 13 ; Carry return
  int 10h

  mov ah, 0x00        ; consume the key
  int 16h
  ret

  .PRESS_ANY_KEY_MSG db "Press any key to continue", 0

; **********
; out: ah (scancode), al (ascii character)
; **********
get_key:
  mov ah, 0x01
  int 16h
  jz get_key  ; wait until key press

  mov ah, 0x00 ; consume the key
  int 16h
  ret
