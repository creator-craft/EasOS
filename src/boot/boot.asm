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
  mov al, 32             ; sectors count
  mov ch, 0              ; low cylinder number
  mov cl, 2              ; sector number (source)
  mov dl, 0x80           ; drive number (0x80 = hard disk)
  mov dh, 0              ; head number
  int 0x13

  jnc load_gdt

  mov di, ax
  cmp ah, 0x0C
  jne disk_error

  mov si, load_msg
  call debug
  mov ax, di

  shr al, 4

  mov dx, 0x3F8
  mov bl, '0'
  mov cl, 'A' - 10
  cmp al, 10
  cmovge bx, cx
  add al, bl
  out dx, al

  mov ax, di
  and al, 0x0F
  mov bl, '0'
  cmp al, 10
  cmovge bx, cx
  add al, bl
  out dx, al

  mov al, 10
  out dx, al

  jmp load_gdt

disk_error:
  mov si, disk_error_msg
  call debug

  mov ax, di
  shr ax, 12
  mov dx, 0x3F8
  mov bl, '0'
  mov cl, 'A' - 10
  cmp al, 10
  cmovge bx, cx
  add al, bl
  out dx, al

  mov ax, di
  shr ax, 8
  and al, 0x0F
  mov bl, '0'
  cmp al, 10
  cmovge bx, cx
  add al, bl
  out dx, al

  mov al, 10
  out dx, al

  hlt
  jmp $

; in: si
debug:
  lodsb
  test al, al
  jz .exit

  mov dx, 0x3F8
  out dx, al

  jmp debug
.exit:
  ret

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

load_msg db "Loaded sectors' count : 0x", 0
disk_error_msg db "Disk error 0x", 0

times 510-($-$$) db 0
dw 0xAA55
