#pragma once
#include <stdint.h>

void keyboard_init(void);
void keyboard_handler(void);

/* prosty bufor znaków (ASCII) */
int  kb_have(void);
char kb_get(void);

/* licznik IRQ1 do debugowania (opcjonalnie wykorzystywany) */
unsigned kb_irq_count(void);

