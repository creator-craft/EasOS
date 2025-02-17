;*********************************************
;  kernel.asm
;
;  The kernel of our system (32 bits mode)
;*********************************************
org 0x0500
[bits  32]

%define KERNEL

; **********
; Kernel entry point
; **********
p_mode:
  mov ax, 16
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  ; mov esp, 0xFFFFFFFF
  ; mov ebp, 0xFFFFFFFF

  ; mov al, 0xF4
  ; out 0x64, al

  call mapPIC

;   mov ecx, 80*25
;   mov edi, 0xB8000
;   mov eax, 0x3000 + ' '
;   rep stosw

;   mov ecx, 256 * 7

; .chars:
;   mov byte [0xB8000+2*ecx], cl

;   loop .chars

;   ; mov byte [0xB8000+2*600], 178

  mov ax, 0x0A00 + ' '
  call clear_screen

  call idt_init

  mov bx, 0
  mov ax, 0
  call set_cursor_pos

  call init_serial

  jmp $

  hlt

HELLO_MSG db "Hello world!", 0

%include "src/boot/globals.asm"
%include "src/kernel/interrupts_handlers.asm"
%include "src/kernel/interrupts.asm"
%include "src/kernel/drivers/PIC.asm"
%include "src/kernel/drivers/text_mode_screen.asm"
%include "src/kernel/drivers/serial.asm"

%macro outb 2
  mov dx, %1
  mov al, %2
  out dx, al
%endmacro

%macro inb 2
  mov dx, %2
  in al, dx
%endmacro

times 16 * 512 - ($ - p_mode) db 0