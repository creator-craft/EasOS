%ifndef DEBUG
%define DEBUG

; **********
;  Prints a string
;  + DS:SI -> 0 terminated string
;  - ax, si
; **********
print:
  mov ah, 0x0e    ; function "Put Character"
  jmp print_start ; go to first character test
print_loop:
  int 10h         ; print character
print_start:
  lodsb           ; AL = [SI++]
  test al, al
  jnz print_loop  ; continue loop while 0 isn't reached

  ret

; **********
;  Prints a short number (hexadecimal representation)
;  + BX -> value to print
;  - ax, si, dx, bx
; **********
hex_values db "0123456789ABCDEF"

print_hex: ; BX: number to print
  mov ah, 0x0e     ; Use function 0x0E : Put Character
  mov si, hex_values
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

%endif