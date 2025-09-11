
global isr_stub_table
extern keyboard_handler, hdc1_handler, hdc2_handler

%macro IRQ_DEF 0
  cli
  jmp PIC_sendEOI
%endmacro

; Exceptions
%assign i 0
%rep 32
  IRQ_%+i: iret
  %assign i i+1
%endrep

; Hardware
IRQ_32: IRQ_DEF

IRQ_33:
  cli
  call keyboard_handler
  jmp PIC_sendEOI

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

IRQ_46:
  cli
  call hdc1_handler
  jmp PIC_sendEOI

IRQ_47:
  cli
  call hdc2_handler
  jmp PIC_sendEOI

; Software
IRQ_N: iret


; **********
; Send an End Of Interrupt signal to PIC (Restore registers, enable interrupts, iret)
; - al
; **********
PIC_sendEOI:
  push ax
  mov al, 0x20
  out 0x20, al
  out 0xA0, al
  pop ax
  sti
  iret

; **********
; Send an End Of Interrupt signal to PIC (Restore registers, enable interrupts, iret)
; - al
; **********
PIC_sendEOI_less8:
  push ax
  mov al, 0x20
  out 0xA0, al
  pop ax
  sti
  iret

isr_stub_table:
  %assign i 0
  %rep 32 + 16
    dd IRQ_%+i
    %assign i i+1
  %endrep

  %rep 256 - %+i
    dd IRQ_N
  %endrep

section .note.GNU-stack noalloc noexec nowrite