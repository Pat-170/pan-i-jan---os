#include <stdint.h>
#include "vga.h"

void kmain(void){
    __asm__ __volatile__("cli");   // absolutnie wyłącz przerwania
    vga_init();
    vga_write("Minimal OS: boot OK\n");
    vga_write("NO IRQ, NO GDT, NO IDT, NO RTC\n");
    vga_write("Stable idle loop...\n");
    for(;;){
        __asm__ __volatile__("hlt");
    }
}
