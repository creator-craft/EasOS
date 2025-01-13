# EasOS

EasOS is a lightweight, educational operating system designed to provide a foundation for learning low-level system programming and OS development. It is built using assembly language for the i386 architecture and is suitable for those who wish to understand the fundamentals of how operating systems work.

## Features

- Written entirely in assembly for i386
- Bootloader implemented using NASM
- Runs on QEMU, ensuring portability and ease of use
- Educational focus with clean and modular code

---

## Getting Started

### Prerequisites

To build and run EasOS, you will need the following tools:

- **NASM**: An assembler for the i386 architecture.
- **QEMU**: A versatile emulator to run and test the OS.
- **Make**: For automating the build process.
- **Git**: For version control and collaboration.

### Clone the Repository

```bashmments.
$ git clone https://github.com/yourusername/easos.git
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

```
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
- [BIOS Interupts Table - from ctyme](http://www.ctyme.com/intr/int.htm)
- [BIOS Interupts Table - from wikipedia](https://en.wikipedia.org/wiki/BIOS_interrupt_call)