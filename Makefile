CROSS?=
CC?=$(CROSS)gcc
LD=$(CROSS)ld

CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra -fno-pic -fno-stack-protector -fno-builtin -nostdlib -nostartfiles
LDFLAGS=-m elf_i386 -T linker.ld

# TYLKO nasze źródła (Drop 0)
SRCS=src/kmain.c src/vga.c src/idt.c src/pic.c src/pit.c src/keyboard.c
ASMS=src/isr.S
OBJS=$(SRCS:.c=.o) $(ASMS:.S=.o)

all: myos.iso

kernel.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

myos.iso: kernel.elf iso/boot/grub/grub.cfg
	mkdir -p iso/boot
	cp kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o $@ iso

# awaryjne uruchomienie bez ISO (gdy nie masz grub-mkrescue/xorriso)
run-elf: kernel.elf
	qemu-system-i386 -kernel kernel.elf -serial stdio -m 64M

run: myos.iso
	qemu-system-i386 -cdrom myos.iso -serial stdio -m 64M

clean:
	rm -f $(OBJS) kernel.elf myos.iso

