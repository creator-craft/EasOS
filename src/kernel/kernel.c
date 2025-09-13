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
  set_PIC_mask(PIC_CASCADE & PIC_KEYBOARD & PIC_PIT, PIC_ATA1);

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

  read_sectors(0x00000000 + (0b0100 << 28), 1, (u32*)0x500);

  print_hex_b(*(u8*)(0x500 + 511)); // IF 0xAA => SUCCESSFUL
}
