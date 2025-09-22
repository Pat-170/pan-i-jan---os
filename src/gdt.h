#pragma once
#include <stdint.h>

void gdt_init(void);

/* Selektory dla 32-bit PM: */
#define GDT_CODE_SEL 0x08
#define GDT_DATA_SEL 0x10
