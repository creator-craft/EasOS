;*********************************************
;  kernel.asm
;
;  The kernel of our system (32 bits mode)
;*********************************************
org 0x500
[bits  32]

%define KERNEL

%macro outb 2
  mov dx, %1
  mov al, %2
  out dx, al
%endmacro

%macro inb 2
  mov dx, %2
  in al, dx
%endmacro

p_mode:
  mov ax, 16
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  ; mov esp, 0xFFFFFFFF
  ; mov ebp, 0xFFFFFFFF

  mov al, 0x22
  call test_command

  jmp $

  mov ah, 0xB0
  call detect_devtype

  mov ebx, 400
  mov eax, 250
  mov ebp, bitmap_font
  mov esi, OTHER_MSG
  test edx, 4
  je .equal
  mov esi, NO_MSG
.equal:
  call print_at

  jmp $

  ; ; call detect_devtype

  ; xor eax, eax

  ; outb 0x01F7, 0xEC

  ; ; mov al, 0xEC
  ; mov dx, 0x01F7
  ; in al, dx

  ; mov bl, al
  ; call print_bar_code

  ; jmp $

OTHER_MSG db "OTHER!", 0
NO_MSG db "PATA/SATA/...", 0

%include "src/boot/globals.asm"
%include "src/kernel/graphics.asm"
%include "src/kernel/ATA.asm"

times 16 * 512 - ($ - p_mode) db 0