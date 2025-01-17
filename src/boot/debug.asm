;*********************************************
;  debug.asm
;
;  Debug utility functions
;*********************************************
[bits  16]
%ifndef DEBUG
%define DEBUG

; ==========
;  print ==> DEFINED AT : bootloader.asm:print
; ==========

; **********
;  Prints a short number (hexadecimal representation)
;  + BX -> value to print
;  - ax, dx, bx
; **********
hex_values db "0123456789ABCDEF"

print_hex: ; BX: number to print
  mov ah, 0x0e     ; Use function 0x0E : Put Character
  mov dx, bx

  shr bx, 12
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  mov bx, dx
  shr bx, 8
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  mov bx, dx
  shr bx, 4
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  mov bx, dx
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  ret

print_sep:
  mov ax, 0x0e00 + ' '
  int 10h
  ret

; struct printing functions

show_struct_begin:

show_struct_byte:
  xor bx, bx
  mov bl, [si]
  inc si
  call print_hex
  jmp print_sep

show_struct_word:
  mov bx, [si]
  add si, 2
  call print_hex
  jmp print_sep

show_struct_double:
  mov bx, [si+2]
  call print_hex
  mov bx, [si]
  add si, 4
  call print_hex
  jmp print_sep

show_struct_end:

%endif