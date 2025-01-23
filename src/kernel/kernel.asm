
org 0x500
[bits 32]

p_mode:
  ; call detect_devtype

  xor eax, eax

  ; mov al, 0xEC
  ; out 0x01F7, al
  in al, 0x0064

  call print_colored_line

  jmp $

; args: eax (color)
print_colored_line:
  mov edi, [mode_info_block.framebuffer]
  mov ecx, 1024 * 100
  rep stosd
  ret



ata_soft_reset:
  push ax

  mov al, 0x04
  out CTRL_DEV_CTL, al

  in al, CTRL_DEV_CTL
  in al, CTRL_DEV_CTL
  in al, CTRL_DEV_CTL
  in al, CTRL_DEV_CTL

  mov al, 0x00
  out CTRL_DEV_CTL, al

  pop ax
  ret

;
; Args: al (slavebit)
;
detect_devtype;
  ; call ata_soft_reset

  mov al, 0xA0

  ; shl al, 4
  ; or al, 0xA0
  out CTRL_BASE + REG_DEVSEL, al

  in al, CTRL_DEV_CTL
  in al, CTRL_DEV_CTL
  in al, CTRL_DEV_CTL
  in al, CTRL_DEV_CTL

  in al, CTRL_BASE + REG_CYL_HIG
  mov ah, al
  in al, CTRL_BASE + REG_CYL_LOW

  jmp print_colored_line

  cmp ax, 0xEB14
  je .PATAPI
  cmp ax, 0x9669
  je .SATAPI
  test ax, ax
  je .PATA
  cmp ax, 0xC33C
  je .SATA

.unknow:
  mov eax, 0xFFFFFF
  jmp print_colored_line
.PATAPI:
  mov eax, 0xFF0000
  jmp print_colored_line
.SATAPI
  mov eax, 0x00FF00
  jmp print_colored_line
.PATA
  mov eax, 0x0000FF
  jmp print_colored_line
.SATA:
  mov eax, 0xFF00FF
  jmp print_colored_line


CTRL_BASE    equ 0x01F0
CTRL_DEV_CTL equ 0x03F6
REG_CYL_LOW  equ 4
REG_CYL_HIG  equ 5
REG_DEVSEL   equ 6

mode_info_block.framebuffer equ 0x7C00

times 16 * 512 - ($ - p_mode) db 0