# Turning it on

1. gcc -m32 -ffreestanding -c src/kernel.o -o kernel.o
2. ld -m elf_i386 -T src/linker.ld -o kernel.elf kernel.o
3. grub-mkrescue -o myos.iso iso/boot/ 

4. qemu-system-i386 -drive format=raw, file=myos.iso (INIT)

# Recompile the files before initialization. (!!!)
