[bits 32]

GLOBAL start_kernel
EXTERN kernel_main

section .start

start_kernel:
  mov ax, 16
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov esp, 0x9FC00

  call kernel_main

halt:
  cli
  hlt
  jmp $

section .note.GNU-stack noalloc noexec nowrite