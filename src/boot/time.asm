;*********************************************
;  time.asm
;
;  Time utility functions
;*********************************************
[bits  16]

; Ret : cx:dx (time ~ 18.2/s), al (is_midnight_passed)
; - ax
get_time:
  mov ax, 0
  int 1Ah   ; get system time
  ret

; - ax, bx, cx, dx
show_time:
  xor ax, ax
  int 1Ah   ; get system time
  mov bx, cx
  mov cx, dx
  call print_hex
  mov bx, cx
  jmp print_hex

; Args: cx:dx (time in microseconds)
; - ah, si
wait_for:
  mov ah, 0x86
  int 15h
  jc .handle_time_error
  ret
.handle_time_error:
  clc
  mov si, .error_message
  jmp println

  .error_message db "Error: Wait already in progress", 0
