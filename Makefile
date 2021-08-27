CC=i686-elf-gcc
ASM=i686-elf-as

FLAGS=-Iinclude -I. -std=gnu99 -ffreestanding -O2 -Wall -Wextra

FILES=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(FILES))

build-grub: build-kernel
	cp myos.bin isodir/boot/myos.bin
	cp grub_stuff/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

build-kernel: $(OBJS)
	$(ASM) boot.s -o boot.o
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $(OBJS) boot.o -lgcc
	if grub-file --is-x86-multiboot myos.bin; then echo multiboot confirmed; else echo the file is not multiboot; fi

run: build-kernel
	qemu-system-x86_64 -m 512 -kernel myos.bin -no-reboot

isorun: build-grub
	qemu-system-x86_64 -m 512 -cdrom myos.iso -no-reboot

%.o: %.c
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.iso