.set MULTIBOOT_MAGIC, 0x1BADB002
.set MULTIBOOT_FLAGS, 0
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot
    .long MULTIBOOT_MAGIC
    .long MULTIBOOT_FLAGS
    .long MULTIBOOT_CHECKSUM

.section .text
.global _start
.extern kmain

# Zmiennie globalne dla C
.global multiboot_magic
.global multiboot_info

_start:
    # GRUB podaje:
    # eax = magic, ebx = mbi ptr

    movl %eax, multiboot_magic
    movl %ebx, multiboot_info

    call kmain

1:  hlt
    jmp 1b

.section .data
multiboot_magic: .long 0
multiboot_info:  .long 0

