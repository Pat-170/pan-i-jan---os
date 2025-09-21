#include <stdint.h>
#include "pit.h"

static inline void outb(uint16_t p, uint8_t v){ __asm__ __volatile__("outb %0,%1"::"a"(v),"Nd"(p)); }

void pit_init(uint32_t hz){
  if(!hz) hz = 100;
  uint32_t div = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, div & 0xFF);
  outb(0x40, (div>>8) & 0xFF);
  // handler i odmaskowanie IRQ0 dodamy w Drop 1
}
