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
  je .end        ; 2
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