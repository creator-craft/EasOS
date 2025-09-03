[bits 32]

; GLOBAL start_kernel
EXTERN kernel_main

start_kernel:
    mov ax, 16
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    call kernel_main

halt:
    hlt
    jmp $

section .note.GNU-stack noalloc noexec nowrite