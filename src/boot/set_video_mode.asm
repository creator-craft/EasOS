
; %include "src/vesa_structs.asm"

%define MIB_WIDTH_OFF 18
%define MIB_HEIGHT_OFF 20
%define MIB_BPP_OFF 25

mode_info_block equ 512

; ---------------
; in:  ax = width, bx = height, cl = bits_per_pixel
; out: cf (clear on success), [mode_info_block]
; ---------------
vbe_set_mode:
  mov word [.width], ax
  mov word [.height], bx
  mov byte [.bpp], cl

  mov ax, 0x4F00      ; get VBE BIOS info
  mov di, vbe_info_block
  int 0x10

  cmp ax, 0x4F        ; BIOS doesn't support VBE?
  jne .error

  mov ax, word [vbe_info_block.video_modes_offset]
  mov si, ax
  mov ax, word [vbe_info_block.video_modes_segment]
  mov fs, ax

.find_mode:
  mov dx, [fs:si]
  mov [.mode], dx
  add si, 2

  cmp dx, 0xFFFF
  je .end_of_list

  mov ax, 0x4F01      ; get VBE mode info
  mov cx, word [.mode]
  mov di, mode_info_block
  int 0x10

  cmp ax, 0x4F
  jne .error

  mov ax, word [.width]
  cmp ax, word [mode_info_block + MIB_WIDTH_OFF]
  jne .find_mode

  mov ax, word [.height]
  cmp ax, word [mode_info_block + MIB_HEIGHT_OFF]
  jne .find_mode

  mov al, byte [.bpp]
  cmp al, byte [mode_info_block + MIB_BPP_OFF]
  jne .find_mode

  mov ax, 0x4F02      ; set VBE video mode
  mov bx, word [.mode]
  or bx, 0x4000       ; enable Linear Frame Buffer
  int 0x10

  cmp ax, 0x4F
  jne .error

  clc
  ret

  .error:
  .end_of_list:
    stc
    ret

  .width   dw 0
  .height  dw 0
  .bpp     db 0
  .mode    dw 0

vbe_info_block:
  .signature dd 0
  .version dw 0
  .oem dd 0
  .capabilities dd 0
  .video_modes_offset dw 0
  .video_modes_segment dw 0
  .video_memory dw 0
  .software_rev dw 0
  .vendor dd 0
  .product_name dd 0
  .product_rev dd 0
  .reserved times 222 db 0
  .oem_data times 256 db 0
