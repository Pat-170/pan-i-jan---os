#include <stdint.h>
#include "pit.h"
#include "pic.h"

/* stan timera */
static volatile unsigned long ticks = 0;
static volatile unsigned long seconds = 0;
static unsigned pit_hz = 100;

/* I/O */
static inline void outb(uint16_t p, uint8_t v){ __asm__ __volatile__("outb %b0,%w1"::"a"(v),"Nd"(p)); }

void pit_init(uint32_t hz){
    if (hz < 19) hz = 100;
    pit_hz = hz;
    uint32_t div = 1193180u / hz;
    outb(0x43, 0x36);                 // kanał0, lobyte/hibyte, tryb 3 (square)
    outb(0x40, (uint8_t)(div & 0xFF));
    outb(0x40, (uint8_t)((div >> 8) & 0xFF));
}

void pit_handler(void){
    ticks++;
    if (ticks % pit_hz == 0) seconds++;
    pic_send_eoi(0);                  // <<< koniecznie
}

unsigned long pit_seconds(void){ return seconds; }

