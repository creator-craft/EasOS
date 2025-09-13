#include "text_mode.h"
#include "interrupts.h"
#include "PCI.h"
#include "ATA.h"
#include "PIT.h"

const char *msg = "Hello from C kernel !\n";

struct ATA_DEVICE_INFORMATION *test = (void*)0x500;

#define CPUID_FEAT_EDX_APIC 0b1000000000000000000000
#define cpuid_d(leaf, edx) __asm__ volatile ("cpuid" : "=d"(edx) : "a"(leaf), "c"(0) : "ebx")

void kernel_main() {
  clear_screen();

  print(msg);

  init_idt();

  map_PIC();
  set_PIC_mask(0b11011000, 0b10111111);

  pit_set_reload(0x0);
  pit_sleep(18);
  print_new_line();

  test_pci();

  // outb(0x3F6, 0x00); // Activate HDC ?

  print_hex_b(identify(0, NULL));

  print_new_line();

  // u32 rep;
  // cpuid_d(1, rep);

  // print_hex_d(rep);

  // print_hex_w(read_pit_count());

  // pit_set_reload(1024);


}
