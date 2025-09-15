
global clock_handler
extern PIC_sendEOI, processes


; in: stack, function
create_process:
  ret

kill_process:
  ret


clock_handler:
  cli
  pushad            ; General
  pushfd            ; Flags
  ; mov ax, ds        ; Segments
  ; push eax
  ; mov ax, es
  ; push eax
  ; mov ax, fs
  ; push eax
  ; mov ax, gs
  ; push eax
  ; fxsave [fpu_state]  ; FPU/MMX/XMM

  ; Context switch
  movzx eax, byte [.current_process_id]
  mov ebx, eax
  mov [processes + 8*ebx], esp ; processes[pid].stack

  ; Find next process
  .find_task:
    inc bl
    cmp eax, ebx
    je .not_found

    mov cl, [processes + 8*ebx + 7] ; processes[tmp_pid].state
    test cl, cl
    jz .find_task

  .found:
    mov [.current_process_id], bl
    mov esp, [processes + 8*ebx]

  .not_found

  ; fxrstor [fpu_state] ; FPU/MMX/XMM
  ; pop eax           ; Segments
  ; mov gs, ax
  ; pop eax
  ; mov fs, ax
  ; pop eax
  ; mov es, ax
  ; pop eax
  ; mov ds, ax
  popfd             ; Flags
  popad             ; General
  jmp PIC_sendEOI

  .current_process_id db 0

section .bss

align 16
fpu_state resb 512

section .note.GNU-stack noalloc noexec nowrite