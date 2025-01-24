;*********************************************
;  tests.asm
;
;  Modules test functions
;*********************************************
[bits  16]

%ifdef TESTS

%ifdef ALL_TESTS
  %define VESA_TESTS
  %define TIME_TESTS
  %define KEY_TESTS
%endif

vesa_tests:
  %ifdef VESA_TESTS
  mov si, .VBE_BLOCK_MSG
  call println
  mov si, vbe_info_block
  call show_VBE_info_struct

  mov si, .VIDEO_MODES_MSG
  call println
  jmp show_video_modes

  .VBE_BLOCK_MSG db "VBE struct:", 0
  .VIDEO_MODES_MSG db "All available video mode:", 0
  %endif
  ret

time_tests:
  %ifdef TIME_TESTS
  mov si, TIME_MSG
  call println
  call show_time

  mov si, .WAIT1S_MSG
  call println
  mov cx, 0x000F ; wait for ~1 second (15 * 65 ms)
  jmp wait_for

  .WAIT1S_MSG db 10, 13, "Waiting ~1 second ...", 0
  %endif
  ret

key_tests:
  %ifdef KEY_TESTS
  jmp .loop_begin

.key_loop:
  mov ah, 0x0E
  int 10h
.loop_begin:
  call get_key
  test al, al
  cmp al, '='
  jne .key_loop

  ret

  .KEY_TEST_MSG db "Press any key to print on the screen", 0
  %endif
  ret

main_test:
  mov si, WELCOME_MSG
  call println

  call vesa_tests
  call time_tests
  call key_tests

  mov si, END_MSG
  call println

  ret


WELCOME_MSG db "=== BEGIN ===", 0
TIME_MSG    db "UTC time (*18.2) : ", 0
END_MSG     db "=== END ===", 0

%endif