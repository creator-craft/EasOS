;*********************************************
;  text_mode_screen.asm
;
;  Screen utility functions for text mode
;*********************************************
[bits 32]

TEXT_SCREEN_BUFFER equ 0xB8000
TEXT_SCREEN_WIDTH equ 80
TEXT_SCREEN_HEIGHT equ 25

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

; **********
; Print text with the current cursor color
; in : esi (char*), [cursor.color] (color)
; - ax, bx, dx, esi, edi
; **********
print:
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

; **********
; Print text without change screen color
; in : esi (char*)
; - ax, ebx, dx, esi
; **********
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

; **********
; Print text without change screen color, and then skip a line
; in : esi (char*)
; - ax, ebx, dx, esi
; **********
println:
  call print_text_only

; **********
; Skip a line
; - ax, bx
; **********
print_new_line:
  mov ax, word [cursor.offset]
  mov bx, TEXT_SCREEN_WIDTH
  div bl
  mul bl
  add bx, ax
  mov word [cursor.offset], bx
  jmp set_cursor_pos.set_offset

; **********
; Print a character on the screen (without change color)
; in : al (char)
; - al, ebx, dx
; **********
print_char:
  movzx ebx, word [cursor.offset]
  mov [TEXT_SCREEN_BUFFER + 2*ebx], al
  inc bx
  mov word [cursor.offset], bx
  jmp set_cursor_pos.set_offset

; **********
; Fill the entire screen with the given character and color
; in : al (char), ah (color)
; - ecx, edi
; **********
clear_screen:
  mov edi, TEXT_SCREEN_BUFFER
  mov ecx, TEXT_SCREEN_WIDTH * TEXT_SCREEN_HEIGHT
  rep stosw
  ret

; **********
; Get the cursor 2D coordinates
; out: ah (x), al (y)
; - ax, dl
; **********
get_cursor_position:
  mov ax, word [cursor.offset]
  mov dl, TEXT_SCREEN_WIDTH
  div dl
  ret


cursor:
  .offset dw 0
  .info db 0
  .shape db 0
  .color db COLOR.BLACK << 4 | COLOR.RED ; BG + FG

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