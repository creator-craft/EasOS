[bits 16]
org 0x7C00

KERNEL_OFFSET equ 0x1000

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov bx, KERNEL_OFFSET  ; destination (es:bx)
    mov ah, 0x02           ; BIOS' function : DISK - READ SECTOR(s) INTO MEMORY
    mov al, 11             ; sectors count
    mov ch, 0              ; low cylinder number
    mov cl, 2              ; sector number (source)
    mov dl, 0x80           ; drive number (0x80 = hard disk)
    mov dh, 0              ; head number
    int 0x13

    jc disk_error

    jmp load_gdt

disk_error:
    hlt
    jmp $


load_gdt:
  cli                      ; disable interrupts
  lgdt [GDT]               ; load GDT

  mov eax, cr0
  or eax, 0b1
  mov cr0, eax

  jmp gdt_cs:KERNEL_OFFSET ; far jump

GDT:                       ; Global Desriptor Table
	.siz dw gdt_size
	.ptr dd GDT_segments
GDT_segments:
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
gdt_size equ (GDT_segments.end - GDT_segments- 1)
gdt_cs   equ (GDT_segments.code - GDT_segments)
gdt_ds   equ (GDT_segments.data - GDT_segments)


times 510-($-$$) db 0
dw 0xAA55
