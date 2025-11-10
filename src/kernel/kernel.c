#include "interrupts.h"
#include "PCI.h"
#include "ATA.h"
#include "PIT.h"
#include "mouse.h"
#include "tests.h"
#include "VBE.h"
#include "screen.h"
#include "resources.h"
#include "debug.h"
#include "processes.h"
#include "input.h"
#include "utils.h"

extern int cursor_x, cursor_y;
int prev_cursor_x = 0, prev_cursor_y = 0;

void kernel_main() {
  init_processes();

  for (int i = 0; i < INPUTS_COUNT; i++)
    processes_inputs[i].pid = UNDEFINED_INPUT;

  processes[0].flags |= PFLAG_KEYBOARD_INPUT;
  // processes_inputs[0] = (struct input) { 0, 0, 0, {}, {} };
  processes_inputs[0].packets_count = 0;
  processes_inputs[0].packets_index = 0;
  processes_inputs[0].pid = 0;

  init_screen();
  init_resources();

  init_idt();

  map_PIC();
  set_PIC_mask(PIC_NONE);

  debug("Mouse init: ");
  debug_hex_b(init_mouse(10, 1));
  debug_new_line();

  set_PIC_mask(PIC_CASCADE & PIC_KEYBOARD & PIC_PIT & PIC_ATA1 & PIC_MOUSE);

  tests();

  debug_hex_b(identify(0, NULL));
  debug_new_line();

  while(1) {
    wait_interrupt(PIC_CASCADE & PIC_KEYBOARD & PIC_MOUSE);

    CLI();
    for (int i = processes_inputs[0].packets_index; i < processes_inputs[0].packets_index + processes_inputs[0].packets_count; i++) {
      debug_hex_b(processes_inputs[0].packets[i].keyboard.scancode);
      debug_new_line();
    }

    processes_inputs[0].packets_index = 0;
    processes_inputs[0].packets_count = 0;
    STI();

    blit_part((struct image) { (u32*)0x100000, mode_info_block->width, mode_info_block->height }, (struct image) { (u32*)mode_info_block->framebuffer, mode_info_block->width, mode_info_block->height }, prev_cursor_x, prev_cursor_y, prev_cursor_x, prev_cursor_y, cursor.width, cursor.height);

    prev_cursor_x = cursor_x;
    prev_cursor_y = cursor_y;

    transparent_blit(cursor, (struct image) { (u32*)mode_info_block->framebuffer, mode_info_block->width, mode_info_block->height }, cursor_x, cursor_y);
  }
}
