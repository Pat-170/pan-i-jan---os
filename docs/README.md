# Turning it on

1. gcc -m32 -ffreestanding -nostartfiles -c src/kernel.c -o objs/kernel.o
2. ld -m elf_i386 -T undefined/linker.ld -o iso/boot/kernel.elf objs/*.o
3. grub-mkrescue -o myos.iso iso/boot/ 
4. qemu-system-i386 -drive format=raw,file=myos.iso (INIT)

// Note: Compile the files you changed.
// GCC: gcc -m32 -ffreestanding -nostartfiles -c src/kernel.c -o objs/kernel.o
// NASM: nasm -f elf32 src/protmode_switch.s -o objs/protmode_switch.o

# Recompile the files before initialization. (!!!)

When you're already in the GNU, you can type "ls".
The first one to appear (in this case it's "(hd0)") has the kernel.elf file inside.
To finally boot, you have to type:

1. multiboot (hd0)/kernel.elf
2. boot

At this point you already booted your kernel.

# 