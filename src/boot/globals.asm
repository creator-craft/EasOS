%ifndef SYSTEM_SHARE
%define SYSTEM_SHARE

KERNEL_OFFSET equ 0x0500

SHARE_BASE equ 0x7C00

shared:
  .framebuffer   equ SHARE_BASE + 0x00 ; 4
  .screen_width  equ SHARE_BASE + 0x04 ; 2
  .screen_height equ SHARE_BASE + 0x06 ; 2
  .screen_pitch  equ SHARE_BASE + 0x08 ; 2
  .screen_bpp    equ SHARE_BASE + 0x0A ; 1
  .refresh_rate  equ SHARE_BASE + 0x0B ; 2

%ifndef KERNEL

%macro MV_d 1
  mov si, %1
  movsd
%endmacro

%macro MV_w 1
  mov si, %1
  movsw
%endmacro

%macro MV_b 1
  mov si, %1
  movsb
%endmacro

; - eax, si, di
proceed_to_sharing:
  mov di, shared.framebuffer
  MV_d mode_info_block.framebuffer
  MV_w mode_info_block.width
  MV_w mode_info_block.height
  MV_w mode_info_block.pitch
  MV_b mode_info_block.bpp
  MV_w CRTC_info_block.rfsh_rate

  ret

%endif

%endif