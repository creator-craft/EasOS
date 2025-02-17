;*********************************************
;  PIC.asm
;
;  Programable Interrupt Controller
;*********************************************

; **********
; Remap PIC interrupts IRQ0:IRQ16 to 32:48
; - al
; **********
mapPIC:
  mov al, ICW_1
  out PIC_1_CTRL, al

  out PIC_2_CTRL, al

  mov al, IRQ_0_OFF
  out PIC_1_DATA, al

  mov al, IRQ_8_OFF
  out PIC_2_DATA, al

  mov al, 0x04       ; 0x04 => 0100, second bit (IR line 2)
  out PIC_1_DATA, al ; write to data register of primary PIC

  mov al, 0x02       ; 010=> IR line 2
  out PIC_2_DATA, al ; write to data register of secondary PIC

  mov al, 1          ; bit 0 enables 80x86 mode
  out PIC_1_DATA, al
  out PIC_2_DATA, al

  mov al, 0
  out PIC_1_DATA, al
  out PIC_2_DATA, al

  ret

; **********
; Set PIC mask, enable/disable interrupts
; in : ah (IRQ 0:8 bits), al (IRQ 8:16 bits)
; - ax
; **********
set_PIC_mask:
  out PIC_1_DATA, al
  shr ax, 8
  out PIC_2_DATA, al
  ret

; **********
; Send an End Of Interrupt signal to PIC (Restore registers, enable interrupts, iret)
; - al
; **********
PIC_sendEOI:
  mov al, 0x20
  out 0x20, al
  out 0xA0, al
  popa
  sti
  iret

; **********
; Send an End Of Interrupt signal to PIC (Restore registers, enable interrupts, iret)
; - al
; **********
PIC_sendEOI_less8:
  mov al, 0x20
  out 0xA0, al
  popa
  sti
  iret


ICW_1      equ 0x11  ; 00010001 binary. Enables initialization mode and we are sending ICW 4
PIC_1_CTRL equ 0x20  ; Primary PIC control register
PIC_2_CTRL equ 0xA0  ; Secondary PIC control register
PIC_1_DATA equ 0x21  ; Primary PIC data register
PIC_2_DATA equ 0xA1  ; Secondary PIC data register

IRQ_0_OFF  equ 0x20  ; IRQs 0-7 mapped to use interrupts 0x20-0x27
IRQ_8_OFF  equ 0x28  ; IRQs 8-15 mapped to use interrupts 0x28-0x36