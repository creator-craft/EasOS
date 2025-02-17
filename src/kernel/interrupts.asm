;*********************************************
;  interrupts.asm
;
;  Load Interrupts Table
;*********************************************

; **********
; Initialize interrupts
; - eax, ebx, ecx, esi, edi
; **********
idt_init:
  mov dword [idtr.base], idt_entries
  mov word [idtr.limit], IDTE_SIZE * 256 - 1

  mov esi, isr_stub_table ; interrupts handlers pointers
  mov edi, idt_entries    ; interrupts table for IDT
  mov ebx, 0x8E000008     ; constant { GDT_CODE_OFFSET: 0x0008, reserved: 0x00, attributes: 0x8E }
  mov ecx, 256

  .idt_set_descriptor:    ; for i in [0:256]: edi[4*i] = esi[4*i][0:2] . 0x8E000008 . esi[4*i][2:4]
    lodsd

    stosw

    xchg eax, ebx
    stosd
    xchg eax, ebx

    shr eax, 16
    stosw

    loop .idt_set_descriptor

  lidt [idtr]
  sti                     ; enable interrupts

  ret


IDTE_SIZE equ 8

idtr:
  .limit dw 0
  .base  dd 0

idt_entries resq 256

isr_stub_table:
  %assign i 0
  %rep 32 + 16
    dd IRQ_%+i
    %assign i i+1
  %endrep

  %rep 256 - %+i
    dd IRQ_N
  %endrep