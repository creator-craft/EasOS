# Directories
SRC_DIR := src
BOOT_DIR := $(SRC_DIR)/boot
KERNEL_DIR := $(SRC_DIR)/kernel
BIN_DIR := bin

# Files
BOOT_SRC := $(BOOT_DIR)/bootloader.asm
KERNEL_SRC := $(KERNEL_DIR)/kernel.asm
BOOT_BIN := $(BIN_DIR)/bootloader.bin
KERNEL_BIN := $(BIN_DIR)/kernel.bin
OS_IMAGE := $(BIN_DIR)/easos.bin

# Tools
ASM := nasm
ASM_FLAGS := -f bin
QEMU := qemu-system-i386

ifeq ($(OS), Windows_NT)
	DEL_FILE := del
	SLASH := "\"
else
	DEL_FILE := rm -rf
	SLASH := /
endif


# Default target
all: OS_IMAGE

# Build the OS image
OS_IMAGE: BOOT_BIN KERNEL_BIN
	@echo "Creating OS image..."
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)

# Build the bootloader
BOOT_BIN:
	@echo "Assembling bootloader..."
	$(ASM) $(ASM_FLAGS) $(BOOT_SRC) -o $(BOOT_BIN)

# Build the kernel
KERNEL_BIN:
	@echo "Assembling kernel..."
	$(ASM) $(ASM_FLAGS) $(KERNEL_SRC) -o $(KERNEL_BIN)

# Run EasOS using QEMU $(OS_IMAGE)
run: OS_IMAGE
	@echo "Running EasOS..."
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)


test:
	@nasm -f bin src/boot/bootloader.asm -o bin/test.bin
	@qemu-system-i386 -drive format=raw,file=bin/test.bin


# Clean build files
clean: BIN_DIR
	@echo "Cleaning up..."
	$(DEL_FILE) $(BIN_DIR)$(SLASH)*.bin

# Ensure the bin directory exists
BIN_DIR:
	@mkdir -p $(BIN_DIR)

# Phony targets
.PHONY: all run clean
