;*********************************************
;  serial.asm
;
;  Serial Communication driver
;*********************************************

SERIAL_PORT equ 0x03F8

init_serial:
  mov dx, SERIAL_PORT + 1
  xor al, al
  out dx, al

  mov dx, SERIAL_PORT + 3
  mov al, 0x80
  out dx, al

  mov dx, SERIAL_PORT
  mov al, 0x03
  out dx, al

  inc dx
  xor al, al
  out dx, al

  inc dx
  inc dx
  mov al, 0x03
  out dx, al

  dec dx
  mov al, 0xC7
  out dx, al

  inc dx
  inc dx
  mov al, 0x0B
  out dx, al

  mov al, 0x1E
  out dx, al

  mov dx, SERIAL_PORT
  mov al, 0xAE
  out dx, al


  in al, dx
  cmp al, 0xAE
  jne .init_error

  mov dx, SERIAL_PORT + 4
  mov al, 0x0F
  out dx, al

  ret

.init_error:
  mov esi, .ERROR_MSG
  jmp println

.ERROR_MSG db "Error: Serial init", 0

; **********
; Write a char into the serial stream
; in : ah (char)
; - al, dx
; **********
write_serial:
  mov dx, SERIAL_PORT + 5
  .while_not_empty:
    in al, dx
    test al, 0x20
    jz .while_not_empty

  mov dx, SERIAL_PORT
  mov al, ah
  out dx, al
  ret

; **********
; Write a 0 terminated string into the serial stream
; in : esi (char*)
; - al, dx, esi
; **********
write_string_serial:
  mov dx, SERIAL_PORT + 5
  .while_not_empty:
    in al, dx
    test al, 0x20
    jz .while_not_empty

  mov dx, SERIAL_PORT
  jmp .loop_start

  .string_loop:
    out dx, al
  .loop_start:
    lodsb
    test al, al
    jnz .string_loop

  ret

  ; **********
; Write a 0 terminated string into the serial stream
; in : ah (value)
; - al, dx, esi
; **********
write_serial_hex_byte:
  mov dx, SERIAL_PORT + 5
  .while_not_empty:
    in al, dx
    test al, 0x20
    jz .while_not_empty

  mov dx, SERIAL_PORT

  mov al, ah
  shr al, 4
  cmp al, 10
  jl .high_less
  add al, 'A' - '9' - 1
.high_less:
  add al, '0'
  out dx, al

  mov al, ah
  and al, 0x0F
  cmp al, 10
  jl .low_less
  add al, 'A' - '9' - 1
.low_less:
  add al, '0'
  out dx, al

  ret

; **********
; Read a char from the serial stream
; out: al (char)
; - al, dx
; **********
read_serial:
  mov dx, SERIAL_PORT + 5
  .while_not_empty:
    in al, dx
    test al, 0x01
    jz .while_not_empty

  mov dx, SERIAL_PORT
  in al, dx
  ret


