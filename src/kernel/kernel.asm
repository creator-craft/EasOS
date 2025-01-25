
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

  mov ebx, 500
  mov eax, 300
  mov ebp, bitmap_font
  mov esi, HELLO_MSG
  call print_at

  jmp $

  ; call detect_devtype

  xor eax, eax

  outb 0x01F7, 0xEC

  ; mov al, 0xEC
  mov dx, 0x01F7
  in al, dx

  mov bl, al
  call print_bar_code

  jmp $

; args: eax (color)
print_colored_line:
  mov edi, [shared.framebuffer]
  mov ecx, 1024 * 100
  rep stosd
  ret

; Args: ebx
print_bar_code:
  mov edi, [shared.framebuffer]
  mov edx, 0x008000
  mov ecx, 8
  .bar_code_loop:
    push ecx

    mov eax, 0x800000
    test bl, 1
    cmovnz eax, edx
    mov ecx, 1024 * 12
    rep stosd

    add edi, 1024*12

    shr bl, 1
    pop ecx
    loop .bar_code_loop
  ret

ata_soft_reset:
  push ax
  mov dx, CTRL_DEV_CTL

  mov al, 0x04
  out dx, al

  in al, dx
  in al, dx
  in al, dx
  in al, dx

  mov al, 0x00
  out dx, al

  pop ax
  ret

;
; Args: al (slavebit)
;
detect_devtype:
  call ata_soft_reset

  mov al, 0xA0

  ; shl al, 4
  ; or al, 0xA0

  outb CTRL_BASE + REG_DEVSEL, al

  mov dx, CTRL_DEV_CTL
  in al, dx
  in al, dx
  in al, dx
  in al, dx

  inb al, CTRL_BASE + REG_CYL_HIG
  mov ah, al
  inb al, CTRL_BASE + REG_CYL_LOW

  cmp ax, 0xEB14
  je .PATAPI
  cmp ax, 0x9669
  je .SATAPI
  test ax, ax
  jz .PATA
  cmp ax, 0xC33C
  je .SATA

.unknow:
  mov eax, 0xFFFFFF
  jmp print_colored_line
.PATAPI:
  mov eax, 0xFF0000
  jmp print_colored_line
.SATAPI:
  mov eax, 0x00FF00
  jmp print_colored_line
.PATA:
  mov eax, 0x00FFFF
  jmp print_colored_line
.SATA:
  mov eax, 0xFF00FF
  jmp print_colored_line


CTRL_BASE    equ 0x01F0
CTRL_DEV_CTL equ 0x03F6
REG_CYL_LOW  equ 4
REG_CYL_HIG  equ 5
REG_DEVSEL   equ 6


%include "src/boot/globals.asm"
%include "src/kernel/graphics.asm"

HELLO_MSG db "Hello world!", 0

times 16 * 512 - ($ - p_mode) db 0