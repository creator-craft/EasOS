;*********************************************
;  bootloader.asm
;
;  Operating Systems Development Tests
;*********************************************
[bits  16]        ; We are still in 16 bit Real Mode
org 0x7C00        ; We are loaded by BIOS at 0x7C00

%define TESTS
; %define ALL_TESTS

start: jmp 0x0000:loader ; Skip to loader and make sure code segment = 0

OS_NAME db "EasOS   "

; **********
;  Prints a string
;  + DS:SI -> 0 terminated string
;  - ax, si
; **********
print:
  mov ah, 0x0E  ; function "Put Character"
  jmp ._start   ; go to first character test
  ._loop:
    int 10h     ; print character
  ._start:
    lodsb       ; AL = [SI++]
    test al, al
    jnz ._loop  ; continue loop while 0 isn't reached
    ret

; **********
;  Prints a short number (hexadecimal representation)
;  + BX -> value to print
;  - ax, bx, dx
; **********
hex_values db "0123456789ABCDEF"

print_hex:
  mov ah, 0x0E ; Use function 0x0E : Put Character
  mov dx, bx   ; save bx for multiple use

  shr bx, 12   ; AND operation isn't required because shift replace left values by 0
  mov al, [hex_values+bx]
  int 10h      ; write hex[bx >> 12]

  mov bx, dx
  shr bx, 8
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h      ; write hex[(bx >> 8) & 0xF]

  mov bx, dx
  shr bx, 4
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h      ; write hex[(bx >> 4) & 0xF]

  mov bx, dx
  and bx, 0x0F ; no shift because we want the last 4 bits
  mov al, [hex_values+bx]
  int 10h      ; write hex[bx & 0xF]

  ret

FIRST_SECTORS_DAP:          ; Disk Address Packet
  .dap_size       db 0x10
  .unused         db 0
  .sectors_count: dw 16     ; number of sectors to be read (sometimes: max is 127)
  .buffer:        dw 0x7E00 ; memory buffer destination address (0:7E00)
                  dw 0      ; in memory page zero
  .start_sector:  dd 1      ; absolute number of the start of the sectors to be read
                  dd 0      ; more storage bytes only for big logical block addressing's ( > 4 bytes )

BOOT_DISK db 0 ; Drive number

; **********
;  Bootloader entry point
; **********
loader:
  xor ax, ax                ; make sure data segments is set to 0
  mov ds, ax
  mov ax, 0x7000            ; set stack position to 7000:FFFF
  mov ss, ax
  mov sp, 0xFFFF
  cld                       ; clear direction flag

  mov [BOOT_DISK], dl       ; save the drive number given by the bios

  mov si, FIRST_SECTORS_DAP ; use our DAP struct
  mov ah, 0x42              ; function "Extended Read Sectors From Drive"
  mov dl, 0x80              ; drive number
  int 13h                   ; ah = status && CF is set on error
  jnc sector2

  push ax
  mov si, .ERROR_MSG
  call print
  pop bx                   ; restore the error code to print it
  call print_hex

  .exit:
    cli
    hlt                     ; halt the system

  .ERROR_MSG db "EasOS can't start : Error while sector loading !", 0

times 510 - ($-$$) db 0 ; We have to be 512 bytes. Clear the rest of the bytes with 0
dw 0xAA55

sector2:
  mov di, vbe_info_block
  call get_VBE_info

  %ifdef TESTS
  call main_test
  %endif

  mov word [VBE_params.width], 1024 ;1280
  mov word [VBE_params.height], 768 ;800
  mov byte [VBE_params.bpp], 32
  call find_mathing_VBE_mode
  mov bx, cx
  call set_VBE_mode

  mov word [DAP_struct.buffer], KERNEL_OFFSET
  mov dword [DAP_struct.start_sector], 17
  mov si, DAP_struct
  call load_sector_dap


  call proceed_to_sharing

  call load_gdt

  cli
  hlt

%include "src/boot/debug.asm"
%include "src/boot/vesa_utils.asm"
%include "src/boot/memory.asm"
%include "src/boot/disk.asm"
%include "src/boot/protected_mode.asm"
%include "src/boot/time.asm"
%include "src/boot/keyboard_utils.asm"
%include "src/boot/tests.asm"
%include "src/boot/globals.asm"

KERNEL_OFFSET equ 0x0500

times 16 * 512 - ($ - sector2) db 0