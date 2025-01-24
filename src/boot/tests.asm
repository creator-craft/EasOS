;*********************************************
;  tests.asm
;
;  Modules test functions
;*********************************************
[bits  16]

%define TESTS

%ifdef TESTS

vesa_tests:
  %ifdef VESA_TESTS
  mov di, vbe_info_block
  call get_VBE_info

  mov si, vbe_info_block
  call show_VBE_info_struct
  %endif
  ret

time_tests:
  %ifdef TIME_TESTS
  mov si, TIME_MSG
  call println

  call show_time

  mov cx, 0x000F ; wait for ~1 second (15 * 65 ms)
  call wait_for
  %endif
  ret

main_test:
  mov si, WELCOME_MSG
  call println

  call vesa_tests

  call time_tests

  mov bx, 0x2626
  mov si, END_MSG
  call println

  ret


WELCOME_MSG db "=== BEGIN ===", 0
TIME_MSG db "UTC time (*18.2) : ", 0
END_MSG db 10, 13, "=== END ===", 0

%endif