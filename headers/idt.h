#ifndef IDT_H
#define IDT_H

#include "/home/kusn/code/kernel/headers/types.h"

// Define the structure of an IDT entry (packed so no padding is added)
typedef struct __attribute__((packed)) {
    uint16_t base_lo;   // Lower 16 bits of the handler function address
    uint16_t sel;       // Kernel segment selector
    uint8_t  always0;   // This must always be 0.
    uint8_t  flags;     // Flags (type and attributes)
    uint16_t base_hi;   // Upper 16 bits of the handler function address
} idt_entry_t;

// Define the pointer structure to load the IDT
typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} idt_ptr_t;

extern idt_entry_t idt_entries[256]; // 256 entries in the IDT
extern idt_ptr_t idt_ptr; // Pointer to the IDT

void idt_install();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

extern void idt_flush(uint32_t);

#endif // IDT_H
