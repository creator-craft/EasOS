#include "tests.h"
#include "types.h"
#include "processes.h"
#include "ATA.h"
#include "debug.h"
#include "PIT.h"

void procA() {
  while (1) {
    for (u32 i = 0; i < 10000000; i++) {
      __asm__ volatile ("wait");
    }

    debug_char('A');
  }
}

void procB() {
  for (u32 t = 0; t < 100; t++) {
    for (u32 i = 0; i < 10000000; i++) {
      __asm__ volatile ("wait");
    }

    debug_char('B');
  }
}

void test_procs() {
  debug("PID: ");
  u8 pid = create_process(procA, (void*)0x8FC00);
  debug_hex_b(pid);
  debug_new_line();
  procB();

  kill_process(pid);

  debug_new_line();
}

void test_ata() {
  // outb(0x3F6, 0x00); // Activate HDC ?

  write_sectors(0x00000004 + (0b0100 << 28), 1, (u32*)0x7C00);

  read_sectors(0x00000004 + (0b0100 << 28), 1, (u32*)0x500);

  debug_hex_b(*(u8*)(0x500 + 511)); // IF 0xAA => SUCCESSFUL
}

#define CPUID_FEAT_EDX_APIC 0b1000000000000000000000
#define cpuid_d(leaf, edx) __asm__ volatile ("cpuid" : "=d"(edx) : "a"(leaf), "c"(0) : "ebx")
void test_cpuid() {
  u32 rep;
  cpuid_d(1, rep);
  debug_hex_d(rep);
}

void test_sleep() {
  pit_set_reload(0x0);
  pit_sleep(18);
}