;*********************************************
;  protected_mode.asm
;
;  Utility functions
;*********************************************
[bits  16]

; **********
; No Return
; **********
load_gdt:
  cli                      ; disable interrupts
  lgdt [GDT]               ; load GDT

  mov eax, cr0
  or eax, 0b1
  mov cr0, eax

  jmp gdt_cs:KERNEL_OFFSET ; far jump



GDT: ; Global Desriptor Table
	.siz dw gdt_size
	.ptr dd GDT_table
GDT_table:
  .null:
    dq 0 ; 2 dd
  .code:
    .limit1     dw 0xFFFF     ; 0-15 limit
    .base1      dw 0x0000     ; 0-15 base
    .base2      db 0x00       ; 16-23 base
    .access     db 0b10011010 ; [ valid, privilege1, privilege2, not_system_segment, executable, direction, readable/writable, accessed ]
    .flags_lmt2 db 0b11001111 ; [ granularity, is_32_bits, is_64_bits, reserved ] << 4 & 16-19 limit
    .base3      db 0x00       ; 24-31 base
  .data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b10010010
    db 0b11001111
    db 0x00
  .end:
gdt_size equ (GDT_table.end - GDT_table - 1)
gdt_cs   equ (GDT_table.code - GDT_table)
gdt_ds   equ (GDT_table.data - GDT_table)