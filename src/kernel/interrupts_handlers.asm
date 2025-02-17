;*********************************************
;  interrupts_handlers.asm
;
;  Every interrupts
;*********************************************

; Each interrupt should be terminated by iret
; And PIC interrupts should call End Of Interrupt before

%assign i 0

%rep 32
  IRQ_%+i: iret
  %assign i i+1
%endrep

%rep 16
  IRQ_%+i:
    pusha
    cli

    %if %+i = 33
      in al, 0x60
      test al, 0b10000000
      jnz .key_released
      mov ah, al
      call write_serial_hex_byte
    .key_released:
    %endif

    mov al, byte [.dat]
    mov byte [0xB8000 + 2 * (%+i - 32)], al
    inc byte [.dat]

    jmp PIC_sendEOI
    .dat db 'A'
  %assign i i+1
%endrep

IRQ_N: iret
