# EasOS

EasOS is a lightweight operating system designed with the goal of mastering low-level system programming and operating system development. It is built using assembly language for the 8086 architecture and C language, from the bootloader to the graphical display, including process management, etc.

## Features

- Written ~~entirely~~ in x86 assembly and C
- Bootloader implemented using NASM
- Runs on QEMU, ensuring portability and ease of use
- Focus on clean and modular code

---

## Getting Started

### Prerequisites

To build and run EasOS, you will need the following tools:

- **NASM**: An assembler for the i386 architecture.
- **GCC**: A compiler for C.
- **QEMU**: A versatile emulator to run and test the OS.
- **Make**: For automating the build process.
- **Git**: For version control and collaboration.

### Clone the Repository

```bash
$ git clone https://github.com/creator-craft/easos.git
$ cd easos
```

### Build the Project

Run the `make` command to assemble and build the OS image:

```bash
$ make
```

This will generate a bootable image file (e.g., `easos.bin`).

### Run EasOS

You can test the OS using QEMU:

```bash
$ make run
```

This will launch QEMU and boot EasOS.

### Clean the Build

To remove generated files, use:

```bash
$ make clean
```

---

## Contributing

Contributions are welcome! If you have ideas for improvements or would like to add features, feel free to fork the repository and open a pull request. Please ensure your contributions align with the educational goals of the project.

### Directory Structure

```yml
EasOS/
├── Makefile        # Build script
├── src/            # Source code for the OS
│   ├── boot/       # Bootloader code
│   └── kernel/     # Kernel code
├── bin/            # Compiled binaries
└── docs/           # Documentation
```

### License

EasOS is released under the [MIT License](LICENSE). Feel free to use, modify, and distribute this project as long as proper attribution is given.

---

## Acknowledgments

EasOS is inspired by many educational OS projects and low-level programming resources. Special thanks to the open-source community for providing invaluable tools and documentation.


### Learning Resources

Here are some resources to help you get started with OS development:

- [The NASM Documentation](https://www.nasm.us/doc/)
- [QEMU User Documentation](https://www.qemu.org/documentation/)
- [OSDev Wiki](https://wiki.osdev.org/)
- [Operating Systems: Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)
- [X86 Instruction Listing](https://en.wikipedia.org/wiki/X86_instruction_listings)
- [BIOS Interrupts Table - from ctyme](http://www.ctyme.com/intr/int.htm)
- [BIOS Interrupts Table - from wikipedia](https://en.wikipedia.org/wiki/BIOS_interrupt_call)
