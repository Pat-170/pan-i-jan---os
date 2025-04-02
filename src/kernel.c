/* multiboot header - This must be at the start of the kernel */

/* 32-bit Multiboot Header */
typedef struct multiboot_header {
    unsigned long magic;
    unsigned long flags;
    unsigned long checksum;
} multiboot_header_t;

__attribute__((section(".multiboot"))) multiboot_header_t header = {
    0x1BADB002,        // Magic number
    0x00000003,        // Flags 
    -(0x1BADB002 + 0x00000003) // Checksum (magic + flags must equal zero)
};

typedef unsigned short uint16_t;

// VGA text mode memory starts at 0xB8000 <--- memorize it ~jakub
#define VIDEO_MEMORY ((uint16_t*) 0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

__attribute__((section(".text"))) 

void clear_screen() {
    uint16_t blank = 0x0F00; // WHITE on BLACK 

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VIDEO_MEMORY[i] = blank;
    }
}

void print_string(const char* str) {
    uint16_t* screen = VIDEO_MEMORY;
    
    while (*str) {
        *screen = (0x0F << 8) | *str;
        screen++;
        str++;
    }
}

void kernel_main() {
    clear_screen();
    print_string("Hello world!\n");

    while (1) {}
}

void _start() {
    kernel_main();
}