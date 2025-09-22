#include <stdint.h>

/* proste I/O */
static inline void outb(uint16_t p, uint8_t v){ __asm__ __volatile__("outb %0,%1"::"a"(v),"Nd"(p)); }
static inline uint8_t inb(uint16_t p){ uint8_t r; __asm__ __volatile__("inb %1,%0":"=a"(r):"Nd"(p)); return r; }

/* poczekaj odrobinę (I/O delay) */
static inline void io_wait(void){ __asm__ __volatile__("outb %0,%1"::"a"(0), "Nd"(0x80)); }

/* ustaw indeks rejestru CMOS (wyłącz NMI – bit 7 = 1) */
static inline void cmos_select(uint8_t reg){
    outb(0x70, (uint8_t)(0x80 | (reg & 0x7F)));
    io_wait();
}

static uint8_t cmos_read(uint8_t reg){
    cmos_select(reg);
    return inb(0x71);
}

/* BCD -> bin */
static uint8_t from_bcd(uint8_t x){
    return (uint8_t)((x & 0x0F) + 10 * ((x >> 4) & 0x0F));
}

/* Uctualizacja RTC w toku? */
static int rtc_updating(void){
    return (cmos_read(0x0A) & 0x80) != 0;  /* bit 7 = Update-In-Progress */
}

/* Publiczne API */
int rtc_is_bcd(void){
    /* Rejestr B: bit 2 = 1 -> binarnie; 0 -> BCD */
    return ((cmos_read(0x0B) & 0x04) == 0);
}

uint8_t rtc_read_reg(uint8_t reg){
    return cmos_read(reg);
}

uint8_t rtc_read_seconds(void){
    /* Czekaj aż nie trwa update */
    while (rtc_updating()) { /* spin */ }
    uint8_t sec = cmos_read(0x00);         /* sekundy */
    if (rtc_is_bcd()) sec = from_bcd(sec);
    return (uint8_t)(sec % 60);
}

uint8_t rtc_wait_next_second(uint8_t cur_sec){
    uint8_t s;
    for(;;){
        while (rtc_updating()) { /* spin */ }
        s = rtc_read_seconds();
        if (s != cur_sec) return s;
    }
}
