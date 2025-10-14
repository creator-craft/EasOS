[bits 32]

GLOBAL start_kernel
EXTERN kernel_main, processes

section .start

start_kernel:
  mov ax, 16
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov esp, 0x9FC00

  mov edi, processes
  mov ecx, 256*2
  rep stosd

  call kernel_main

halt:
  ; hlt
  jmp $

section .note.GNU-stack noalloc noexec nowrite