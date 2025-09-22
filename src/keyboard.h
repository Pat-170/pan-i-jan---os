#pragma once
#include <stdint.h>

void keyboard_init(void);
void keyboard_handler(void);

int  kb_have(void);
char kb_get(void);
unsigned kb_irq_count(void);

