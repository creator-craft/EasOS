;*********************************************
;  bootloader.asm
;
;  Operating Systems Development Tests
;*********************************************
[bits  16]        ; We are still in 16 bit Real Mode
org 0x7C00        ; We are loaded by BIOS at 0x7C00

start: jmp 0x0000:loader ; Skip to loader and make sure code segment = 0

OS_NAME db "EasOS   "

; **********
;  Prints a string
;  + DS:SI -> 0 terminated string
;  - ax, si
; **********
print:
  mov ah, 0x0e  ; function "Put Character"
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
;  - ax, dx, bx
; **********
hex_values db "0123456789ABCDEF"

print_hex:
  mov ah, 0x0e ; Use function 0x0E : Put Character
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

  ; xor  ax, ax               ; clear ax
  ; int  0x12                 ; get the amount of KB from the BIOS
  ; mov bx, ax
  ; call Print_hex

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
  mov si, WELCOME_MSG
  call print

  mov di, vbe_info_block
  call get_VBE_info

  mov si, vbe_info_block
  call show_VBE_info_struct

  call print_new_line

  mov word [VBE_params.width], 1024 ;1280
  mov word [VBE_params.height], 768 ;800
  mov byte [VBE_params.bpp], 32
  call find_mathing_VBE_mode
  mov bx, cx
  ; call set_VBE_mode


  mov si, TIME_MSG
  call println

  mov ax, 0
  int 1Ah   ; get system time
  push dx
  mov bx, cx
  call print_hex
  pop bx
  call print_hex

  mov cx, 0x000F ; wait for ~1 second
  mov ah, 0x86
  int 15h

  call print_new_line
  mov si, mode_info_block.framebuffer
  call show_struct_double

  mov word [DAP_struct.buffer], KERNEL_OFFSET
  mov dword [DAP_struct.start_sector], 17
  mov si, DAP_struct
  call load_sector_dap

  mov bx, 0x2626
  mov si, END_MSG
  call println

  mov eax, [mode_info_block.framebuffer]
  mov dword [0x7C00], eax
  mov eax, [mode_info_block.width]
  mov dword [0x7C04], eax

  ; call load_gdt

  call detect_memory

  cli
  hlt

TIME_MSG db "UTC time (*18.2) : ", 0
WELCOME_MSG db "Hello World", 0
END_MSG db 10, 13, "=== END ===", 0

%include "src/boot/debug.asm"
%include "src/boot/vesa_utils.asm"
%include "src/boot/memory.asm"
%include "src/boot/disk.asm"
%include "src/boot/protected_mode.asm"

KERNEL_OFFSET equ 0x0500

times 16 * 512 - ($ - sector2) db 0