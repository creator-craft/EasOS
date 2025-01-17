;*********************************************
;  vesa_utils.asm
;
;  Utility functions for VESA (Video Electronics Standard Association).
;*********************************************
[bits  16]
%include "src/boot/debug.asm"

vbe_info_block:
  .signature    dd 0x00000000
  .version      dw 0
  .oem          dd 0
  .capabilities dd 0
  .video_modes  dd 0
  .video_memory dw 0
  .software_rev dw 0
  .vendor       dd 0
  .product_name dd 0
  .product_rev  dd 0
  .reserved     times 222 db 0x
  .oem_data     times 256 db 0

mode_info_block:
  .attributes   dw 0
  .window_a     db 0
  .window_b     db 0
  .granularity  dw 0
  .window_size  dw 0
  .segment_a    dw 0
  .segment_b    dw 0
  .win_func_ptr dd 0
  .pitch        dw 0

  .width        dw 0 ; screen width
  .height       dw 0 ; screen height
  .w_char       db 0
  .y_char       db 0
  .planes       db 0
  .bpp          db 0 ; bits per pixel
  .banks        db 0
  .memory_model db 0
  .bank_size    db 0
  .image_pages  db 0
  .reserved0    db 0

  .red_mask                db 0
  .red_position            db 0
  .green_mask              db 0
  .green_position          db 0
  .blue_mask               db 0
  .blue_position           db 0
  .reserved_mask           db 0
  .reserved_position       db 0
  .direct_color_attributes db 0

  .framebuffer             dd 0
  .off_screen_mem_off      dd 0
  .off_screen_mem_size     dw 0
  .reserved1     times 206 db 0

CRTC_info_block:
  .h_pixels     dw 0
  .h_sync_start dw 0
  .h_sync_end   dw 0
  .scan_lines   dw 0
  .v_sync_start dw 0
  .v_sync_end   dw 0
  .byte_flags   db 0 ; Bitfields : { 0: enable double scanning, 1: enable interlacing, 2: h sync polarity, 3: v sync polarity }
  .px_clock     dd 0 ; Hz
  .rfsh_rate    dw 0 ; 0.01 Hz units
  .reserved     times 40 db 0


VBE_params:
  .width  dw 0
  .height dw 0
  .bpp    db 0


VBE_NOT_SUPPORTED_MSG db "BIOS doesn't support VBE", 10, 0

test_VBE_error:
  cmp ax, 0x004F         ; AH = { 0: SUCCESS, 1: FAILED, 2: FN_UNSUPPORTED, 3: INVALID_FN }
  je .no_error

  stc
  mov si, VBE_NOT_SUPPORTED_MSG
  call print
  .no_error:
    ret


; **********
;
; **********
get_VBE_info:
  mov ax, 0x4F00
  mov di, vbe_info_block
  int 10h

  jmp test_VBE_error

; **********
;
; + cx: mode
; - ax, di
; **********
get_VBE_mode_info:
  mov ax, 0x4F01
  ; mov cx, [.mode]
  mov di, mode_info_block
  int 10h

  jmp test_VBE_error


; **********
;
; **********
does_VBE_mode_match:
  

; **********
; Find a VBE mode matching with width, height and bpp given in VBE_params struct.
; Args: [VBE_params]
; Ret : cx (mode not found = FFFF)
; **********
find_mathing_VBE_mode:
  mov si, [vbe_info_block.video_modes]
  test si, si
  jz .no_match

  mov ax, 0x4F01
  mov di, mode_info_block

  jmp .match_start
  .match_loop:
    int 10h
    ; call test_VBE_error

    mov bx, [VBE_params.width]
    cmp bx, [mode_info_block.width]
    jne .match_start

    mov bx, [VBE_params.height]
    cmp bx, [mode_info_block.height]
    jne .match_start

    mov bl, [VBE_params.bpp]
    cmp bl, [mode_info_block.bpp]
    jne .match_start

    ret ; match with mode : cx
  .match_start:
    mov cx, [si]
    add si, 2
    cmp cx, 0xFFFF
    jne .match_loop
  .no_match:
    mov cx, 0xFFFF
    ret

; **********
; Args: bx (video_mode)
;
; **********
set_VBE_mode:
  xor ax, ax
  mov es, ax
  mov ax, 0x4F02
  mov di, CRTC_info_block ; for (VBE 3.0+)
  int 10h

  jmp test_VBE_error

; **********
;
;
; **********
print_VBE_info:



  ret