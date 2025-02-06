;*********************************************
;  text_mode_screen.asm
;
;  Screen utility functions for text mode
;*********************************************
[bits 32]

TEXT_SCREEN_BUFFER equ 0xB8000
TEXT_SCREEN_WIDTH equ 80
TEXT_SCREEN_HEIGHT equ 25

Cursor:

; **********
; in : bx (y), ax (y)
; - ax, bx, dx
; **********
set_cursor_pos:
  mov dl, TEXT_SCREEN_WIDTH
  mul dl
  add bx, ax

; **********
; in : bx (cursor position)
; - al, dx
; **********
.set_offset:
  mov dx, 0x03D4
  mov al, 0x0F
  out dx, al

  inc dl
  mov al, bl
  out dx, al

  dec dl
  mov al, 0x0E
  out dx, al

  inc dl
  mov al, bh
  out dx, al
  ret

; **********
; - al, dx
; **********
disable_cursor:
  mov dx, 0x03D4
  mov al, 0x0A ; low cursor shape register
  out dx, al

  inc dl
  in al, dx

  or al, 0b00100000 ; { 6-7: unused, 5: disable_cursor, 0-4: cursor_shape }
  out dx, al

  ret

; **********
; - al, dx
; **********
enable_cursor:
  mov dx, 0x03D4
  mov al, 0x0A  ; low cursor shape register
  out dx, al

  inc dl
  in al, dx

  and al, 0b11011111
  out dx, al

  ret

; **********
; in : bl (start line), bh (end line)
; - al, dx
; **********
set_cursor_shape:
  mov dx, 0x03D4
  mov al, 0x0A
  out dx, al

  inc dl
  in al, dx
  and al, 0b11100000
  or al, bl
  out dx, al

  dec dl
  mov al, 0x0B
  out dx, al

  inc dl
  in al, dx
  and al, 0b11100000
  or al, bh
  out dx, al

  ret


; ------------------------------
;       NON IO FUNCTIONS
; ------------------------------

; esi
print: ; with defined color
  movzx edi, word [cursor.offset]
  shl edi, 1
  add edi, TEXT_SCREEN_BUFFER
  mov ah, [cursor.color]
  jmp .loop_begin

  .for_loop:
    stosw
  .loop_begin:
    lodsb
    test al, al
    jnz .for_loop

  sub edi, TEXT_SCREEN_BUFFER
  shr di, 1
  mov bx, di
  mov word [cursor.offset], bx
  jmp set_cursor_pos.set_offset

; esi
print_text_only:
  movzx ebx, word [cursor.offset]
  jmp .loop_begin

  .for_loop:
    mov byte [TEXT_SCREEN_BUFFER + 2*ebx], al
    inc ebx
  .loop_begin:
    lodsb
    test al, al
    jnz .for_loop

  mov word [cursor.offset], bx
  jmp set_cursor_pos.set_offset

println:
  call print_text_only

print_new_line:
  mov ax, word [cursor.offset]
  mov bx, TEXT_SCREEN_WIDTH
  div bl
  mul bl
  add bx, ax
  mov word [cursor.offset], bx
  jmp set_cursor_pos.set_offset


; al
print_char:
  movzx ebx, word [cursor.offset]
  mov [TEXT_SCREEN_BUFFER + 2*ebx], al
  inc bx
  mov word [cursor.offset], bx
  jmp set_cursor_pos.set_offset


; ax
clear_screen:
  mov edi, TEXT_SCREEN_BUFFER
  mov ecx, TEXT_SCREEN_WIDTH * TEXT_SCREEN_HEIGHT
  rep stosw
  ret

; out: ah (x), al (y)
get_cursor_position:
  mov ax, word [cursor.offset]
  mov dl, TEXT_SCREEN_WIDTH
  div dl
  ret


cursor:
  .offset dw 0
  .info db 0
  .shape db 0
  .color db COLOR.BLACK << 4 | COLOR.RED

COLOR:
  .BLACK         equ 0x0
  .BLUE          equ 0x1
  .GREEN         equ 0x2
  .CYAN          equ 0x3
  .RED           equ 0x4
  .MAGENTA       equ 0x5
  .BROWN         equ 0x6
  .LIGHT_GRAY    equ 0x7
  .DARK_GRAY     equ 0x8
  .LIGHT_BLUE    equ 0x9
  .LIGHT_GREEN   equ 0xA
  .LIGHT_CYAN    equ 0xB
  .LIGHT_RED     equ 0xC
  .LIGHT_MAGENTA equ 0xD
  .YELLOW        equ 0xE
  .WHITE         equ 0xF