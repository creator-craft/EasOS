;*********************************************
;  bootloader.asm
;
;  Operating Systems Development Tests
;*********************************************
[bits  16]        ; We are still in 16 bit Real Mode
org 0x7C00        ; We are loaded by BIOS at 0x7C00

start: jmp loader ; Skip data section

OS_NAME db "EasOS   "

; **********
;  Prints a string
;  + DS:SI -> 0 terminated string
;  - ax, si
; **********
print:
  mov ah, 0x0e    ; function "Put Character"
  jmp ._start ; go to first character test
  ._loop:
    int 10h         ; print character
  ._start:
    lodsb           ; AL = [SI++]
    test al, al
    jnz ._loop  ; continue loop while 0 isn't reached
    ret

FIRST_SECTORS_DAP:          ; Disk Address Packet
  .dap_size       db 0x10
  .unused         db 0
  .sectors_count: dw 128    ; number of sectors to be read (sometimes: max is 128)
  .buffer:        dw 0x7E00 ; memory buffer destination address (0:7E00)
                  dw 0      ; in memory page zero
  .start_sector:  dd 1      ; absolute number of the start of the sectors to be read
                  dd 0      ; more storage bytes only for big logical block addressing's ( > 4 bytes )

; **********
;  Bootloader entry point
; **********
loader:
  xor ax, ax ; make sure data segment is set to 0
  mov ds, ax
  cld        ; clear direction flag

  ; xor  ax, ax    ; clear ax
  ; int  0x12      ; get the amount of KB from the BIOS
  ; mov bx, ax
  ; call Print_hex

  mov si, FIRST_SECTORS_DAP ; use our DAP struct
  mov ah, 0x42              ; function "Extended Read Sectors From Drive"
  mov dl, 0x80              ; drive number
  int 13h                   ; ah = status && CF is set on error
  jnc sector2

  mov si, .ERROR_MSG
  call print

  .exit:
    cli
    hlt ; halt the system
  
  .ERROR_MSG db "EasOS can't start : Error while sector loading !", 0

times 510 - ($-$$) db 0 ; We have to be 512 bytes. Clear the rest of the bytes with 0
dw 0xAA55

sector2:
  mov si, WELCOME_MSG
  call print

  mov word [VBE_params.width], 800
  mov word [VBE_params.height], 600
  mov word [VBE_params.bpp], 24
  call find_mathing_VBE_mode

  mov bx, cx
  call print_hex

  cli
  hlt

WELCOME_MSG db "Hello World", 0

%include "src/boot/debug.asm"
%include "src/boot/disk.asm"
%include "src/boot/vesa_utils.asm"

times 128 * 512 - ($ - sector2) db 0