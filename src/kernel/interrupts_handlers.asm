
global clock_handler
extern PIC_sendEOI, processes_registers, clock, current_process_id

debug_hex_d:
  push dx
  push eax
  push ecx

  mov dx, 0x3F8

  mov ecx, 8
  .hex_loop:
    mov eax, ebx
    shr eax, 28
    and al, 0x0F
    cmp al, 10
    jg .greater
      add al, '0' - ('A' - 10)
    .greater
      add al, 'A' - 10
    out dx, al
    shl ebx, 4
    loop .hex_loop

  mov al, 10
  out dx, al

  pop ecx
  pop eax
  pop dx
  ret

clock_handler:
  cli
  push ebp

  movzx ebp, byte [current_process_id] ; processes[pid].registers
  shl ebp, 5
  add ebp, processes_registers

  ; Save general registers
  mov dword [ebp + 0], eax
  mov dword [ebp + 4], ebx
  mov dword [ebp + 8], ecx
  mov dword [ebp + 12], edx

  pop eax
  mov dword [ebp + 16], eax ; ebp
  mov dword [ebp + 20], esi
  mov dword [ebp + 24], edi
  mov dword [ebp + 28], esp

  ; Segments (ds, es, fs, gs)
  ; fxsave [fpu_state]  ; FPU/MMX/XMM

  ; Context switch

  call clock
  mov ebp, eax
  shl ebp, 5
  add ebp, processes_registers

  ; fxrstor [fpu_state] ; FPU/MMX/XMM
  ; Segments

  ; Resore general registers
  ; Guess: ebp = processes[pid].registers

  mov eax, dword [ebp + 0]
  mov ebx, dword [ebp + 4]
  mov ecx, dword [ebp + 8]
  mov edx, dword [ebp + 12]

  mov esi, dword [ebp + 20]
  mov edi, dword [ebp + 24]
  mov esp, dword [ebp + 28]
  mov ebp, dword [ebp + 16]

  jmp PIC_sendEOI

  ; .current_process_id db 0

section .bss

align 16
fpu_state resb 512

section .note.GNU-stack noalloc noexec nowrite