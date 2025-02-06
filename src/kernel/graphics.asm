;*********************************************
;  graphics.asm
;
;  Drawing functions for graphical mode
;*********************************************
[bits  32]

; **********
; Print a binary value on screen
; in : eax (y), ebx (x), ecx (value)
; - eax, ebx, ecx, edx, edi
; **********
print_binary:
  push ebp
  push ecx
  std

  movzx edx, word [shared.screen_width]
  mov ebp, edx
  add ebp, 32*10
  shl ebp, 2 ; ebp = (1024 + (32 * 10)) * 4
  push ebp

  mul edx
  mov edi, eax
  add edi, ebx
  shl edi, 2
  add edi, dword [shared.framebuffer]
  add edi, (32*10 - 2) * 4

  mov ecx, 8
  .height_loop:
    mov ebp, ecx

    mov edx, [esp+4]
    mov ecx, 32
    .bit_loop:
      mov ebx, ecx
      mov ecx, 8

      mov eax, 0xA05030
      test edx, 1
      jz .bit_clear
      mov eax, 0x00FF40
    .bit_clear:
      rep stosd

      sub edi, 8
      shr edx, 1
      mov ecx, ebx
      loop .bit_loop

    add edi, dword [esp+0]
    mov ecx, ebp
    loop .height_loop

  add esp, 8
  pop ebp
  cld
  ret


; **********
; Print a string on the screen at a the position (ebx, eax)
; in : esi (char*), ax (y), ebx (x), ebp (font)
; - eax, ebx, ecx, edx, esi, edi, ebp
; **********
print_at:
  movzx edx, word [shared.screen_width]
  mul edx
  mov edi, eax
  add edi, ebx
  shl edi, 2
  add edi, dword [shared.framebuffer]

  movzx edx, word [shared.screen_width]

  mov bh, byte [ebp+0]
  shl edx, 5
  cmp bh, 16
  jne .8bytes_font
  shl edx, 1
.8bytes_font:
  sub edx, 8 * 4
  push edx ; edx = (screen_width * font_size - 8) * bpp

  inc ebp
  push ebp

  xor edx, edx
  mov dx, word [shared.screen_pitch]
  sub edx, 8*4

  xor eax, eax
  jmp ._start
  ._loop:
    mov ebp, esi
    call print_char_at
    sub edi, [esp+4] ; (1024 * 16 - 8) * 4
    mov esi, ebp
  ._start:
    lodsb       ; AL = [ESI++]
    test al, al
    jnz ._loop  ; continue loop while 0 isn't reached

  add esp, 8
  ret


; **********
; Print a string on the screen at a the position (ebx, eax)
; in : al (character (should be 0 extended)), edi (framebuffer), edx, [esp+4] (font_ptr), bh (font size)
; - eax, ebx, ecx, edx, esi, edi
; **********
print_char_at:
  mov esi, eax
  shl esi, 4 ; * 16
  add esi, [esp+4]

  movzx ecx, bh
  ._height_loop:
    push ecx

    lodsb
    mov bl, al
    mov ecx, 8
    .width_loop:
      mov al, bl
      and eax, 0b1
      neg eax
      ; and eax, 0xFF0000
      stosd

      shr bl, 1
      loop .width_loop

    add edi, edx
    pop ecx
    loop ._height_loop

  ret


bitmap_font:
  .sze db 16
  .ptr incbin "res/VGA-8x16_.font" ; from https://github.com/epto/epto-fonts