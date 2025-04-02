# Turning it on

1. gcc -m32 -ffreestanding -c src/kernel.c -o kernel.o
2. ld -m elf_i386 -T src/linker.ld -o iso/boot/kernel.elf kernel.o
3. grub-mkrescue -o myos.iso iso/boot/ 

4. qemu-system-i386 -drive format=raw,file=myos.iso (INIT)

# Recompile the files before initialization. (!!!)

When you're already in the GNU, you can type "ls".
The first one to appear (in this case it's "(hd0)") has the kernel.elf file inside.
To finally boot, you have to type:

1. multiboot (hd0)/kernel.elf
2. boot

At this point you already booted your kernel.
