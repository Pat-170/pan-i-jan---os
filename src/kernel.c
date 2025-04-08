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

#include "/home/kusn/code/kernel/headers/video.h"
#include "/home/kusn/code/kernel/headers/idt.h"

// VGA text mode memory starts at 0xB8000 <--- memorize it ~jakub
#define VIDEO_MEMORY ((uint16_t*) 0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

__attribute__((section(".text"))) 

extern void clear_screen();
extern void print_string();
extern void protmodesetup();
extern void idt_install();
extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

extern idt_entry_t idt_entries[256];
extern idt_ptr_t idt_ptr;

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

    //idt_install();

    
    while (1) {}
}