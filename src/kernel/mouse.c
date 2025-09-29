#include "io.h"
#include "debug.h"

#define MOUSE_DATA_PORT   0x60
#define MOUSE_STATUS_PORT 0x64

#define MOUSE_SAFETY_REPEAT_COUNT 10000

enum MOUSE_CONSTANTS {
  MOUSE_SEND_COMMAND = 0xD4,

  MOUSE_CMD_RESET = 0xFF,
  MOUSE_CMD_RESEND = 0xFE,
  MOUSE_CMD_SET_DEFAULTS = 0xF6,
  MOUSE_CMD_DISABLE_PACKET_STREAMING = 0xF5,
  MOUSE_CMD_ENABLE_PACKET_STREAMING = 0xF4,
  MOUSE_CMD_SET_SAMPLE_RATE = 0xF3,
  MOUSE_CMD_GET_MOUSE_ID = 0xF2,
  MOUSE_CMD_REQUEST_SINGLE_PACKET = 0xEB,
  MOUSE_CMD_STATUS_REQUEST = 0xE9,
  MOUSE_CMD_SET_RESOLUTION = 0xE8,

  MOUSE_ACK = 0xFA,
  MOUSE_SELF_TEST = 0xAA,

  MOUSE_STATUS_READ_BUFFER = 0b00000001,
  MOUSE_STATUS_WRITE_BUFFER = 0b00000010,
};

void mouse_wait_read() {
  for (u32 i = 0; i < MOUSE_SAFETY_REPEAT_COUNT; i++)
    if (inb(MOUSE_STATUS_PORT) & MOUSE_STATUS_READ_BUFFER)
      return;
}

void mouse_wait_write() {
  for (u32 i = 0; i < MOUSE_SAFETY_REPEAT_COUNT; i++)
    if (!(inb(MOUSE_STATUS_PORT) & MOUSE_STATUS_WRITE_BUFFER))
      return;
}

u8 mouse_wait_ACK() {
  mouse_wait_read();
  u8 res = inb(MOUSE_DATA_PORT);
  if (res == MOUSE_ACK)
    return 0;
  if (res == MOUSE_CMD_RESEND)
    return 2;
  return res;
}

u8 mouse_send_command(u8 val) {
  mouse_wait_write();
  outb(MOUSE_STATUS_PORT, MOUSE_SEND_COMMAND);
  mouse_wait_write();
  outb(MOUSE_DATA_PORT, val);
  return mouse_wait_ACK();
}

u8 mouse_id, mouse_sample_rate, mouse_resolution;

u8 init_mouse(u8 sample_rate, u8 resolution) { // Default: 10, 0x01
  u8 tmp;
  // Enable Mouse device
  mouse_wait_write();
  outb(MOUSE_STATUS_PORT, 0xA8);

  // Enable mouse's interrupt
  mouse_wait_write();
  outb(MOUSE_STATUS_PORT, 0x20);
  mouse_wait_read();
  tmp = (inb(MOUSE_DATA_PORT) | 0b00000010) & 0b11011111; // Enable interrupt & enable mouse
  mouse_wait_write();
  outb(MOUSE_STATUS_PORT, 0x60);
  mouse_wait_write();
  outb(MOUSE_DATA_PORT, tmp);

  // Config
  tmp = mouse_send_command(MOUSE_CMD_RESET);
  if (tmp == 0) {
    mouse_wait_read();
    if (inb(MOUSE_DATA_PORT) != MOUSE_SELF_TEST)
      return 1;
  } else if (tmp != MOUSE_SELF_TEST) // ACK might not be sent
    return 2;
  mouse_wait_read();
  mouse_id = inb(MOUSE_DATA_PORT);

  if (  mouse_send_command(MOUSE_CMD_SET_DEFAULTS) ||
        mouse_send_command(MOUSE_CMD_SET_SAMPLE_RATE) ||
        mouse_send_command(sample_rate) ||
        mouse_send_command(MOUSE_CMD_SET_RESOLUTION) ||
        mouse_send_command(resolution) ||
        mouse_send_command(MOUSE_CMD_ENABLE_PACKET_STREAMING)  )
    return 3;

  mouse_sample_rate = sample_rate;
  mouse_resolution = resolution;

  return 0;
}

u8 mouse_enable_scroll_wheel() {
  if (  mouse_send_command(MOUSE_CMD_SET_SAMPLE_RATE) ||
        mouse_send_command(200) ||
        mouse_send_command(MOUSE_CMD_SET_SAMPLE_RATE) ||
        mouse_send_command(100) ||
        mouse_send_command(MOUSE_CMD_SET_SAMPLE_RATE) ||
        mouse_send_command(80) )
    return 4;

  if (mouse_send_command(MOUSE_CMD_GET_MOUSE_ID))
      return 5;
  mouse_wait_read();
  mouse_id = inb(MOUSE_DATA_PORT);

  if (mouse_id == 0x03 || mouse_id == 0x04)
      return 0;
  return 6;
}
