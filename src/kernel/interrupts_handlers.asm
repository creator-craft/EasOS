
global clock_handler
extern PIC_sendEOI, test_handler


processus:
  stack dd 0
  priority db 0
  cycles dw 0


clock_handler:
  push eax ; C_ABI caller-saved registers
  push ecx
  push edx
  call test_handler
  pop edx
  pop ecx
  pop eax

  jmp PIC_sendEOI
  cli
  pushad            ; General
  pushfd            ; Flags
  mov ax, ds        ; Segments
  push eax
  mov ax, es
  push eax
  mov ax, fs
  push eax
  mov ax, gs
  push eax
  ; fxsave [fpu_state]  ; FPU/MMX/XMM

  ; Context switch

  ; fxrstor [fpu_state] ; FPU/MMX/XMM
  pop eax           ; Segments
  mov gs, ax
  pop eax
  mov fs, ax
  pop eax
  mov es, ax
  pop eax
  mov ds, ax
  popfd             ; Flags
  popad             ; General
  jmp PIC_sendEOI

section .bss
  align 16
  fpu_state resb 512

section .note.GNU-stack noalloc noexec nowrite