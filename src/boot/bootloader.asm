;*********************************************
;  bootloader.asm
;
;  Operating Systems Development Tests
;*********************************************

[bits  16]        ; We are still in 16 bit Real Mode
org 0x7c00        ; We are loaded by BIOS at 0x7C00

start: jmp loader ; Skip data section

OS_NAME db "EasOS   "
error_msg db "Error while sector loading, code : 0x", 0 ; the string to print

; **********
;  Prints a string
;  + DS:SI -> 0 terminated string
;  - ax, si
; **********
Print:
  mov ah, 0eh     ; function "Put Character"
  jmp Print_start ; go to first character test
Print_loop:
  int 10h         ; print character
Print_start:
  lodsb           ; AL = [SI++]
  test al, al
  jnz Print_loop  ; continue loop while 0 isn't reached

  ret


; **********
;  Prints a short number (hexadecimal representation)
;  + BX -> value to print
;  - ax, si, dx, bx
; **********
hex_values db "0123456789ABCDEF"

Print_hex: ; BX: number to print
  mov ah, 0eh     ; Use function 0x0E : Put Character
  mov si, hex_values
  mov dx, bx

  shr bx, 12
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  mov bx, dx
  shr bx, 8
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  mov bx, dx
  shr bx, 4
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  mov bx, dx
  and bx, 0x0F
  mov al, [hex_values+bx]
  int 10h

  ret


FIRST_SECTORS_DAP:          ; Disc Address Packet
  .dap_size       db 0x10
  .unused         db 0
  .sectors_count: dw 16     ; number of sectors to be read (sometimes: max is 127)
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
  int 0x13
  jc short error            ; show error message + code if carry

  jmp sector2                 ; jump to the 1st sector


error:
  push ax           ; save error code
  mov si, error_msg
  call Print
  pop bx            ; restore and show it
  call Print_hex

exit:
  cli
  hlt ; halt the system
WELCOME_MSG db "Hello World", 0

times 510 - ($-$$) db 0 ; We have to be 512 bytes. Clear the rest of the bytes with 0
dw 0xAA55

sector2:
  mov si, WELCOME_MSG
  call Print
  cli
  hlt

times 16 * 512 - ($ - sector2) db 0
