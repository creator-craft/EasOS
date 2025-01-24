[bits  32]

; Args: ebx
print_bar_code:
  mov edi, [mode_info_block.framebuffer]
  mov edx, 0x008000
  mov ecx, 8
  .bar_code_loop:
    push ecx

    mov eax, 0x800000
    test bl, 1
    cmovnz eax, edx
    mov ecx, 1024 * 12
    rep stosd

    add edi, 1024*12

    shr bl, 1
    pop ecx
    loop .bar_code_loop
  ret