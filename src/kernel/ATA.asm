;*********************************************
;  ATA.asm
;
;  A
;*********************************************

; **********
; - al, dx
; **********
ata_soft_reset:
  mov dx, CTRL_DEV_CTL

  mov al, 0x04
  out dx, al

  in al, dx
  in al, dx
  in al, dx
  in al, dx

  mov al, 0x00
  out dx, al

  ret

; **********
; Args: ah (slavebit [0xA0: master | 0xB0: slave])
; Ret : edx (type [0: PATAPI | 1: SATAPI | 2: PATA | 3: SATA | 4: UNKNOW])
; **********
detect_devtype:
  call ata_soft_reset

  outb CTRL_BASE + REG_DEVSEL, ah

  mov dx, CTRL_DEV_CTL
  in al, dx
  in al, dx
  in al, dx
  in al, dx

  inb al, CTRL_BASE + REG_CYL_HIG
  mov ah, al
  inb al, CTRL_BASE + REG_CYL_LOW

  xor edx, edx
  cmp ax, 0xEB14
  je .end        ; 0
  inc edx
  cmp ax, 0x9669
  je .end        ; 1
  inc edx
  test ax, ax
  jz .end        ; 2
  inc edx
  cmp ax, 0xC33C
  je .end        ; 3
  inc edx
  ; UNKNOW =>      4
.end:
  ret

CTRL_BASE    equ 0x01F0
CTRL_DEV_CTL equ 0x03F6
REG_CYL_LOW  equ 4
REG_CYL_HIG  equ 5
REG_DEVSEL   equ 6



; ; **********
; ; in : al
; ; out: al
; ; **********
; test_command:
;   ; outb 0x
;   ; outb HDC_COMMAND, Al

;   outb HDC_DRIVE_HEAD, 0xA0
;   wait

;   outb HDC_SECTOR_COUNT, 0
;   wait
;   outb HDC_SECTOR_NB, 0
;   wait
;   outb HDC_CYL_LOW, 0
;   wait
;   outb HDC_CYL_HIGH, 0
;   wait

;   outb HDC_COMMAND, 0xEC

;   inb al, HDC_STATUS
;   test al, al
;   jz .cmd_error
;   test al, 0b1
;   jnz .cmd_error

;   mov ecx, 4
; .busy_loop:
;   in al, dx
;   test al, 0b10000000
;   jnz .busy_loop

;   inb al, HDC_DATA
;   mov ecx, 0xFF
;   and ecx, eax
;   mov ebx, 400
;   mov eax, 250
;   call print_binary

;   inb al, HDC_SECTOR_COUNT
;   mov ecx, 0xFF
;   and ecx, eax
;   mov ebx, 400
;   mov eax, 260
;   call print_binary

;   inb al, HDC_SECTOR_NB
;   mov ecx, 0xFF
;   and ecx, eax
;   mov ebx, 400
;   mov eax, 270
;   call print_binary

;   inb al, HDC_CYL_LOW
;   mov ecx, 0xFF
;   and ecx, eax
;   mov ebx, 400
;   mov eax, 280
;   call print_binary

;   inb al, HDC_CYL_HIGH
;   mov ecx, 0xFF
;   and ecx, eax
;   mov ebx, 400
;   mov eax, 290
;   call print_binary

;   ret

; .cmd_error:
;   mov ebp, bitmap_font
;   mov esi, .ERROR_MSG
;   mov ebx, 400
;   mov eax, 250
;   call print_at
;   ret

; .ERROR_MSG db "ATA call error", 0




HDC_DATA         equ 0x01F0 ; 16 bits
HDC_ERROR        equ 0x01F1 ; [ bad_block, uncorrecable_ECC_error, ID_found, command_completed, track_000_not_found, DAM_not_found ]
HDC_WPC          equ 0x01F1 ; Write Precompensation Cylinder divided by 4
HDC_SECTOR_COUNT equ 0x01F2 ; When 0: count = 256
HDC_SECTOR_NB    equ 0x01F3 ; LBA Low
HDC_CYL_LOW      equ 0x01F4 ; LBA Mid
HDC_CYL_HIGH     equ 0x01F5 ; LBA High
HDC_DRIVE_HEAD   equ 0x01F6 ; [ 101, 4: drive, 0-3: head ]
HDC_STATUS       equ 0x01F7 ; [ busy, drive_ready, write_fault, seek_complete, buffer_requires_servicing, data_readed, index, has_previous_command_error ]
HDC_COMMAND      equ 0x01F7 ; Commands