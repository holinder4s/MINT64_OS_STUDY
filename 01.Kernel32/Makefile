####################################################################
# 빌드 환경 및 규칙 설정
####################################################################
NASM32 = nasm
GCC32 = x86_64-pc-linux-gcc -c -m32 -ffreestanding
LD32 = x86_64-pc-linux-ld -melf_i386 -T ../elf_I386.x -nostdlib -e Main -Ttext 0x10200
OBJCOPY32 = x86_64-pc-linux-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJECTDIRECTORY = Temp
SOURCEDIRECTORY = Source

all: Kernel32.bin

Kernel32.bin: Source/EntryPoint.s
	nasm -o Kernel32.bin $<

clean:
	rm -f Kernel32.bin