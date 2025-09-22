#pragma once
#include <stdint.h>

#define PIC1_CMD   0x20
#define PIC1_DATA  0x21
#define PIC2_CMD   0xA0
#define PIC2_DATA  0xA1
#define PIC_EOI    0x20

void pic_remap(int offset1, int offset2);
void irq_mask(uint8_t irq);
void irq_unmask(uint8_t irq);
void pic_send_eoi(uint8_t irq);
void pic_mask_all(void);
void pic_unmask_all(void);

