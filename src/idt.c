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
