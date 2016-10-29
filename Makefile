# OSX? Check out:
# https://github.com/cfenollosa/os-tutorial/tree/master/11-kernel-crosscompiler

CC=/usr/local/i386elfgcc/bin/i386-elf-gcc-4.9.1
LD=/usr/local/i386elfgcc/bin/i386-elf-ld
OBJCOPY=/usr/local/i386elfgcc/bin/i386-elf-objcopy

CFLAGS=-c -g -Os -m32 -march=i386 -ffreestanding -Wall -Werror -I.
LDFLAGS=-static -Tlinker.ld -nostdlib --nmagic -melf_i386

IMAGE=bootblock.img

.PHONY: all image clean qemu bochs

all: image

bootblock: bootblock.o
	$(LD) $(LDFLAGS) -o $@.elf $^
	$(OBJCOPY) -O binary $@.elf $@.bin

image: bootblock
	dd if=/dev/zero of=$(IMAGE) bs=1024 count=1440
	dd if=bootblock.bin of=$(IMAGE) bs=1 count=512 conv=notrunc

qemu:
	qemu-system-i386 -fda $(IMAGE) -boot a

bochs:
	bochs -f .bochsrc_no_debug

clean:
	-$(RM) *.o
	-$(RM) *.elf
	-$(RM) bootblock
	-$(RM) $(IMAGE)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<

#gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
#$ ld -static -Tlinker.ld -nostdlib --nmagic -o vbr.elf vbr.o
#$ objcopy -O binary vbr.elf vbr.bin