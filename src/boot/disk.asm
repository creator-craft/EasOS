;*********************************************
;  disk.asm
;
;  All kind of disk utility functions such as loading or writing sectors on disk
;*********************************************
[bits  16]

loading_sector_error_msg db "Error while sector loading, code : 0x", 0 ; the string to print

DAP_struct:          ; Disk Address Packet
  .dap_size       db 0x10
  .unused         db 0
  .sectors_count: dw 16     ; number of sectors to be read (sometimes: max is 127)
  .buffer:        dw 0x7E00 ; memory buffer destination address (0:7E00)
                  dw 0      ; in memory page zero
  .start_sector:  dd 1      ; absolute number of the start of the sectors to be read
                  dd 0      ; more storage bytes only for big logical block addressing's ( > 4 bytes )

; **********
; - ax, bx, dx, si
; **********
show_DAP_struct:
  call show_struct_byte
  call show_struct_byte
  call show_struct_word
  call show_struct_word
  call show_struct_word
  call show_struct_double
  jmp show_struct_double

; **********
;
; Args: ax (error_code)
; **********
sector_error:
  push ax           ; save error code
  mov si, loading_sector_error_msg
  call print
  pop bx            ; restore and show it
  jmp print_hex

; **********
;
; Args: si (DAP*)
; Ret : CF (on error)
; **********
load_sector_dap:
  mov ah, 0x42      ; function "Extended Read Sectors From Drive"
  mov dl, 0x80      ; drive number
  int 13h
  jc sector_error   ; show error message + code if carry
  ret
