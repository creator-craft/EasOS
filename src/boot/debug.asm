;*********************************************
;  debug.asm
;
;  Debug utility functions
;*********************************************
[bits  16]

; ==========
;  print ==> DEFINED AT : bootloader.asm:print (0x7C0A)
;  print_hex ==> DEFINED AT : bootloader.asm:print_hex (0x7C26)
; ==========

; **********
; Print a string termitated by a 0 and then a new line.
; Args: si (char*)
; - ax, si
; **********
println:
  call print

print_new_line:
  mov ax, 0x0E00 + 10
  int 10h
  mov al, 13
  int 10h
  ret

; **********
; - ax
; **********
print_sep:
  mov ax, 0x0e00 + ' '
  int 10h
  ret

; struct printing functions

; **********
; Print a structure byte/word/double and move the pointer.
; Args: si (struct*)
; - ax, bx, dx, si
; **********
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
