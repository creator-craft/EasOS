
org 0x500
[bits 32]

mode_info_block.framebuffer equ 0x7C00

p_mode:
  mov edi, [mode_info_block.framebuffer]
  mov eax, 0x0000FF
  mov ecx, 1024 * 3
  rep stosd

  jmp $

times 16 * 512 - ($ - p_mode) db 0