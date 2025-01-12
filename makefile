
all:
	nasm -f bin -o bin/boot.bin src/boot.asm
	nasm -f bin -o bin/sector2.bin src/sector2.asm
	copy boot.bin /B + sector2.bin /B os.bin /B
	qemu-system-i386 .\bin\os.bin