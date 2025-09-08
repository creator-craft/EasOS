ENTRY(start_kernel)

SECTIONS
{
  . = 0x1000;

  .text : {
    KEEP(*(.start))
    *(.text)
  }

  .rodata : {
    *(.rodata)
  }

  .data : {
    *(.data)
  }

  .bss : {
    *(.bss)
    *(COMMON)
  }
}
