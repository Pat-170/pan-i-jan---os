#pragma once
#include <stdint.h>
void pic_remap(int off1, int off2);
void pic_set_mask(uint16_t mask);
void pic_eoi(int irq);
