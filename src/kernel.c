/* multiboot header - This must be at the start of the kernel */

/* 32-bit Multiboot Header */
typedef struct multiboot_header {
    unsigned long magic;
    unsigned long flags;
    unsigned long checksum;
} multiboot_header_t;

__attribute__((section(".multiboot"))) multiboot_header_t header = {
    0x1BADB002,        // Magic number
    0x00000003,        // Flags (memory and video info)
    -(0x1BADB002 + 0x00000003) // Checksum (magic + flags must equal zero)
};

__attribute__((section(".text"))) 
__attribute__((naked))

void _start() {
    *(char*)0xB8000 = 'H'; 
    *(char*)0xB8001 = 0x07; // WHITE ON BLACK
    while (1) {} 
}
