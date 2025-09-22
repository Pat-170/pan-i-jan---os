#include <stdint.h>
#include "pic.h"

/* I/O (wymuszone rozmiary rejestrów: AL/DX) */
static inline void outb(uint16_t p, uint8_t v){
    __asm__ __volatile__("outb %b0, %w1" :: "a"(v), "Nd"(p));
}
static inline uint8_t inb(uint16_t p){
    uint8_t r; __asm__ __volatile__("inb %w1, %0" : "=a"(r) : "Nd"(p));
    return r;
}
static inline void io_wait(void){
    __asm__ __volatile__("outb %b0, %w1" :: "a"(0), "Nd"(0x80));
}

/* Remap PICów na wektory 0x20..0x2F */
void pic_remap(int offset1, int offset2){
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_CMD, 0x11); io_wait();
    outb(PIC2_CMD, 0x11); io_wait();

    outb(PIC1_DATA, (uint8_t)offset1); io_wait();
    outb(PIC2_DATA, (uint8_t)offset2); io_wait();

    outb(PIC1_DATA, 0x04); io_wait(); // PIC2 na IRQ2
    outb(PIC2_DATA, 0x02); io_wait();

    outb(PIC1_DATA, 0x01); io_wait(); // 8086/88 mode
    outb(PIC2_DATA, 0x01); io_wait();

    outb(PIC1_DATA, a1);             // przywróć maski
    outb(PIC2_DATA, a2);
}

/* Maskowanie/odmaskowanie pojedynczego IRQ */
void irq_mask(uint8_t irq){
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    uint8_t v = inb(port) | (uint8_t)(1u << irq);
    outb(port, v);
}
void irq_unmask(uint8_t irq){
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    uint8_t v = inb(port) & (uint8_t)~(1u << irq);
    outb(port, v);
}

/* Zbiorczo */
void pic_mask_all(void){
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}
void pic_unmask_all(void){
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);
}

/* EOI dla kontrolera/kaskady */
void pic_send_eoi(uint8_t irq){
    if (irq >= 8) outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

