;*********************************************
;  memory.asm
;
;  Utility functions to detect available memory
;*********************************************
[bits 16]

BIOS_SMMARD: ; System Memory Map Address Range Descriptor
  .base_address  dq 0
  .region_length dq 0
  .type          dw 0 ; { 1: Usable memory (free), 2:  Unusable memory (reserved), 3: ACPI reclaimed memory, 4, ACPi NVS memory, 5: Bad memory, _: Undefined (reserved) }
  .ACPI_EAB      dw 0 ; Extended Attribute Bitfield

memory_map_error db "memory error", 0

; **********
; - ax, bx, dx, si
; **********
show_SMMARD_struct:
  mov si, BIOS_SMMARD
  call show_struct_qword
  call show_struct_qword
  call show_struct_double
  jmp print_new_line


; **********
; Print a list of memory regions that can be used or not.
; - eax, ebx, ecx, edx
; **********
detect_memory:
  xor ebx, ebx        ; should be 0 to start reading the list
  mov ecx, 24         ; count of bits we want for the struct (20 or 24)

.list_memory:
  mov eax, 0xE820     ; memory map function (make sure upper bits of eax should be 0)
  mov edx, 534D4150h  ; magic number
  mov di, BIOS_SMMARD ; destination struct
  int 15h

  jc .mem_err
  pusha               ; save registers (because bx and cx should be conserved)
  call show_SMMARD_struct
  popa

  test ebx, ebx       ; the list finish on ebx = 0, and then restart if if call the function again
  jnz .list_memory

  ret

.mem_err:
  clc                 ; clear the carry flag
  mov si, memory_map_error
  jmp println         ; print the error

; **********
; This function returns the amount of kilobytes of contiguous memory starting at absolute address 0x0000 (also found at word [0x0040:0x0013])
; out: ax
; **********
get_bios_memory_size:
  xor  ax, ax         ; clear ax
  int  0x12           ; get memory size
  ret

; **********
; - ax, bx, dx
; **********
show_bios_memory_size:
  xor  ax, ax
  int  0x12
  mov bx, ax
  jmp print_hex
