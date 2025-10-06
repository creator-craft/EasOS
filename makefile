NASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-i386

CFLAGS = -ffreestanding -fno-pic -m32 -Wall -Wextra -O2 -Iinclude
LDFLAGS = -m elf_i386 -T linker.ld
RES_OCFLAGS = -I binary -O elf32-i386 -B i386
#  --oformat binary

OBJ_DIR = obj
BIN_DIR = bin
SRC_DIR = src
RES_DIR = res

# List of source files
RES = $(wildcard $(RES_DIR)/*)
BOOT_SRCS := $(wildcard $(SRC_DIR)/boot/*.asm)
ASM_SRCS = $(wildcard $(SRC_DIR)/kernel/*.asm)
C_SRCS   = $(wildcard $(SRC_DIR)/kernel/*.c)

# Corresponding objects
RES_OBJ  = $(patsubst $(RES_DIR)/%,$(OBJ_DIR)/%.o,$(RES))
ASM_OBJS = $(patsubst $(SRC_DIR)/kernel/%.asm,$(OBJ_DIR)/%_asm.o,$(ASM_SRCS))
C_OBJS   = $(patsubst $(SRC_DIR)/kernel/%.c,$(OBJ_DIR)/%_c.o,$(C_SRCS))
OBJS     = $(RES_OBJ) $(ASM_OBJS) $(C_OBJS)

# Kernel binary
KERNEL_ELF = $(OBJ_DIR)/kernel.elf
KERNEL_BIN = $(OBJ_DIR)/kernel.bin

# Bootloader binary
BOOT_BIN = $(OBJ_DIR)/boot.bin

# Output image
OS_IMG = $(BIN_DIR)/os.img

all: $(OS_IMG)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Bootloader asm -> bin
$(BOOT_BIN): $(BOOT_SRCS) | $(OBJ_DIR)
	$(NASM) -f bin $(SRC_DIR)/boot/boot.asm -o $(OBJ_DIR)/boot.bin

# ASM -> ELF
$(OBJ_DIR)/%_asm.o: $(SRC_DIR)/kernel/%.asm | $(OBJ_DIR)
	$(NASM) -f elf32 $< -o $@

# C -> ELF
$(OBJ_DIR)/%_c.o: $(SRC_DIR)/kernel/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(RES_DIR)/% | $(OBJ_DIR)
	$(OBJCOPY) $(RES_OCFLAGS) $< $@

# Link the kernel
$(KERNEL_BIN): $(OBJS) | $(OBJ_DIR)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJS)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $@

# Concate binaries into the final image
$(OS_IMG): $(BOOT_BIN) $(KERNEL_BIN) | $(BIN_DIR)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $@

# Start QEMU
run: $(OS_IMG)
	$(QEMU) -drive format=raw,file=$(OS_IMG) -serial stdio

# Clean objects
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
