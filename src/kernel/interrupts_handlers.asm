;*********************************************
;  interrupts_handlers.asm
;
;  Every interrupts
;*********************************************

; Each interrupt should be terminated by iret
; And PIC interrupts should call End Of Interrupt before

%macro IRQ_DEF 0
  cli
  pusha
  jmp PIC_sendEOI
%endmacro

%assign i 0

%rep 32
  IRQ_%+i: iret
  %assign i i+1
%endrep

IRQ_32: IRQ_DEF
IRQ_33 equ keyboard_handler
IRQ_34: IRQ_DEF
IRQ_35: IRQ_DEF
IRQ_36: IRQ_DEF
IRQ_37: IRQ_DEF
IRQ_38: IRQ_DEF
IRQ_39: IRQ_DEF
IRQ_40: IRQ_DEF
IRQ_41: IRQ_DEF
IRQ_42: IRQ_DEF
IRQ_43: IRQ_DEF
IRQ_44: IRQ_DEF
IRQ_45: IRQ_DEF
IRQ_46: IRQ_DEF
IRQ_47: IRQ_DEF

IRQ_N: iret


keyboard_handler:
  cli
  pusha

  xor eax, eax
  in al, 0x60
  test al, 0b10000000
  jnz .end ; key released

  mov bl, al
  mov al, byte [AZERTY_X2_KEYMAP + 4*eax]
  test al, al
  jz .special_key

  call print_char
  jmp .end

.special_key:
  

.end:
  jmp PIC_sendEOI

