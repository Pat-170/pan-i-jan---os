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

// VGA text mode memory starts at 0xB8000 <--- memorize it ~jakub
#define VIDEO_MEMORY ((uint16_t*) 0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

__attribute__((section(".text"))) 

int is_protected_mode_set = 0;

void _start() {
    clear_screen();

    print_string("Welcome!\n");

    if (!is_protected_mode_set) {
        is_protected_mode_set = 1;
        protmodesetup();
        
    } else {
        print_string("System (GDT included) initialized!");
    }

    idt_install();

    
    while (1) {}
}