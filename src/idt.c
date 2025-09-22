#include <stdint.h>
#include "idt.h"

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(int n, uint32_t base, uint16_t sel, uint8_t flags){
  idt[n].base_lo = base & 0xFFFF;
  idt[n].sel     = sel;
  idt[n].always0 = 0;
  idt[n].flags   = flags;
  idt[n].base_hi = (base >> 16) & 0xFFFF;
}

extern void idt_load(struct idt_ptr*);

void idt_init(void){
  idtp.limit = sizeof(idt)-1;
  idtp.base  = (uint32_t)&idt;

  for(int i=0;i<256;i++)
    idt_set_gate(i, 0, 0x08, 0x8E); // placeholdery

  idt_load(&idtp);
  __asm__ __volatile__("sti");
}

extern void irq0(void);
extern void irq1(void);

void idt_install_irqs(void){
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E); // PIT
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E); // Keyboard
}

