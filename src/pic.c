#include <stdint.h>
#include "pic.h"

static inline void outb(uint16_t p, uint8_t v){ __asm__ __volatile__("outb %0,%1"::"a"(v),"Nd"(p)); }
static inline uint8_t inb(uint16_t p){ uint8_t r; __asm__ __volatile__("inb %1,%0":"=a"(r):"Nd"(p)); return r; }

#define PIC1      0x20
#define PIC2      0xA0
#define PIC1_CMD  PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_CMD  PIC2
#define PIC2_DATA (PIC2+1)
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void pic_remap(int off1, int off2){
  uint8_t a1=inb(PIC1_DATA), a2=inb(PIC2_DATA);
  outb(PIC1_CMD, ICW1_INIT|ICW1_ICW4);
  outb(PIC2_CMD, ICW1_INIT|ICW1_ICW4);
  outb(PIC1_DATA, off1);
  outb(PIC2_DATA, off2);
  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);
  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);
  outb(PIC1_DATA, a1);
  outb(PIC2_DATA, a2);
}
void pic_set_mask(uint16_t mask){
  outb(PIC1_DATA, mask & 0xFF);
  outb(PIC2_DATA, (mask>>8) & 0xFF);
}

void pic_eoi(int irq){
  if(irq>=8) outb(PIC2_CMD, 0x20);
  outb(PIC1_CMD, 0x20);
}

