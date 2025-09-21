#include <stdint.h>
#include "vga.h"
#include "idt.h"
#include "pit.h"
#include "keyboard.h"

void kmain(void){
  vga_init();
  vga_write("Minimal OS: boot OK\n");

  idt_init();
  vga_write("IDT ready\n");

  pit_init(100);
  vga_write("PIT 100Hz\n");

  keyboard_init();
  vga_write("Keyboard IRQ1\n");

  for(;;){ __asm__ __volatile__("hlt"); }
}
