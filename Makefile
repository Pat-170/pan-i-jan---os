CC      := gcc
AS      := gcc
LD      := ld

CFLAGS  := -m32 -ffreestanding -O2 -Wall -Wextra -fno-pic -fno-stack-protector -fno-builtin -nostdlib -nostartfiles
ASFLAGS := -m32
LDFLAGS := -m elf_i386 -T linker.ld

# dopasuj listę do tego co masz – ważne by multiboot_header.o było pierwsze
OBJ := src/multiboot_header.o src/start.o src/vga.o \
       src/gdt.o src/gdt_flush.o src/idt.o src/isr.o \
       src/pic.o src/pit.o src/keyboard.o \
       src/kmain.o


all: kernel.elf

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.S
	$(AS) $(ASFLAGS) -c $< -o $@

kernel.elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

iso: kernel.elf
	rm -rf iso
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	printf 'set timeout=2\nset default=0\nmenuentry "kernel-os" {\n\tmultiboot /boot/kernel.elf\n\tboot\n}\n' > iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso

run: iso
	qemu-system-i386 -cdrom os.iso -boot d -no-reboot -no-shutdown -serial stdio

clean:
	rm -f src/*.o kernel.elf os.iso
	rm -rf iso

