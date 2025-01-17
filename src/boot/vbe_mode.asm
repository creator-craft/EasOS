;*********************************************
;  vbe_mode.asm
;
;  Activate video mode
;*********************************************

; **********
;  Activate video mode
;  + AX -> Width
;  + BX -> Height
;  + CL -> Bits per pixel
;  - ax, si, dx, bx
; **********
vbe_set_mode: