.intel_syntax noprefix
.code32

/* Minimalny nagłówek Multiboot v1 (GRUB2) */
.set MAGIC,    0x1BADB002
.set FLAGS,    0x00000000          /* żadnych dodatkowych żądań */
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot, "a"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
