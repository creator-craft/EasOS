[bits 32]

GLOBAL start_kernel
EXTERN kernel_main

section .start

start_kernel:
  mov ax, 16
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov esp, 0x90000

  mov dx, 0x3F8
  mov al, 'M'
  out dx, al
  call kernel_main

  halt:
  hlt
  jmp $

section .note.GNU-stack noalloc noexec nowrite