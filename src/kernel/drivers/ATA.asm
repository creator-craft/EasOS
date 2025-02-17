;*********************************************
;  ATA.asm
;
;  AT Atachment driver implementation
;*********************************************

; ===  d1410r3b documentation  ===


ATA_reg_data         equ 0x01F0
ATA_reg_error        equ 0x01F1
ATA_reg_sector_count equ 0x01F2
ATA_reg_LBA_low      equ 0x01F3
ATA_reg_LBA_mid      equ 0x01F4
ATA_reg_LBA_high     equ 0x01F5
ATA_reg_device       equ 0x01F6
ATA_reg_status       equ 0x01F7
ATA_reg_command      equ 0x01F7


; **********
; Wait until device is no longer busy
; - al, dx, esi*, CF
; **********
ATA_wait_busy:
  mov dx, ATA_reg_status
.busy_loop:
  in al, dx
  test al, 0b10000000
  jnz .busy_loop

  test al, 0b00000001
  jnz .error

  ret
.error:
  stc
  mov esi, .error_message
  jmp println
.error_message db "ATA Error while waiting for busy"

; **********
; Pre-erases and conditions from 1 to 256 sectors. Should be issued before a CFA write without erase
; in : al (sector count), ebx (device + LBA)
; **********
cfa_erase_sectors: ret
  ; mov dx, ATA_reg_status
  ; in al, dx
  ; test al, 0b01000000
  ; jz ATA_drive_not_ready

  mov dx, ATA_reg_sector_count
  out dx, al

  mov eax, ebx
  inc
  out dx, al

  shr eax, 8
  inc dl
  out dx, al

  shr eax, 8
  inc dl
  out dx, al

  mov al, ah
  inc dl
  out dx, al

  mov al, 0xC0
  inc dl
  out dx, al

  jmp ATA_wait_busy.busy_loop


; **********
; Provides an extended error code which identifies the cause of an error
; in : al (device)
; out: al (error) { 00: No error detected, 01: Self-test passed, 03: Write / Erase failed, 05: Self-test or diagnostic failed, 09: Miscellaneous error, 0B: Vendor specific, 0C: Corrupted media format, 0D-0F: Vendor specific, 10: ID Not Found / ID Error, 11: Uncorrectable ECC error, 14: ID Not Found, 18: Corrected ECC error, 1D | 1E: Vendor specific, 1F: Data transfer error / command aborted, 20: Invalid command, 21: Invalid address, 22-23: Vendor specific, 27: Write protect violation, 2F: Address overflow (address too large), 30-34: Self-test or diagnostic failed, 35 | 36: Supply or generated voltage out of tolerance, 37 | 3E: Self-test or diagnostic failed, 38: Corrupted media format, 39: Vendor specific, 3A: Spare sectors exhausted, 3B | 3Ch | 3F: Corrupted media format, 3D: Vendor specific, All other value: Reserved }
; **********
cfa_request_extended_error_code: ret

; **********
; Provides information to a specific sector
; in : al (device + LBA)
; out: [edi] (cfa_translate_sector information)
; **********
cfa_translate_sector: ret

; **********
; Similar to write_multiple. Sectors should be pre-erased
; in : eax (device + LBA), cl (sector count)
; **********
cfa_write_multiple_without_erase: ret

cfa_write_sectors_without_erase: ret

check_media_card_type: ret

check_power_mode: ret

device_configuration: ret

device_reset: ret

download_microcode: ret

execute_device_diagnostic: ret

flush_cache: ret

flush_cache_ext: ret

get_media_status: ret

identify_device: ret

identify_packet_device: ret

idle: ret

idle_immediate: ret

media_eject: ret

media_lock;

media_unlock: ret

ATA_nop: ret

packet: ret

read_buffer: ret

read_dma: ret

read_dma_ext: ret

read_dma_queued: ret

read_dma_queued_ext: ret

read_log_ext: ret

read_multiple: ret

read_multiple_ext: ret

read_native_max_address: ret

read_native_max_address_ext: ret

read_sectors: ret

read_sectors_ext: ret

read_verify_sectors: ret

read_verify_sectors_ext: ret
