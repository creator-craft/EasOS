;*********************************************
;  bootloader.asm
;
;  Operating Systems Development Tests
;*********************************************
[bits  16]        ; We are still in 16 bit Real Mode
org 0x7C00        ; We are loaded by BIOS at 0x7C00

start: jmp loader ; Skip data section

OS_NAME db "EasOS   "

%include "src/boot/debug.asm"
%include "src/boot/disk.asm"

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
  call load_sector_dap

  jnc sector2                 ; jump to the 1st sector

exit:
  cli
  hlt ; halt the system
WELCOME_MSG db "Hello World", 0

times 510 - ($-$$) db 0 ; We have to be 512 bytes. Clear the rest of the bytes with 0
dw 0xAA55

sector2:
  mov si, WELCOME_MSG
  call print
  cli
  hlt

times 16 * 512 - ($ - sector2) db 0
