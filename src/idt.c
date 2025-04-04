#include "/home/kusn/code/kernel/undefined/idt.h"

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

extern void idt_flush(uint32_t);
extern void keyboard_handler();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void idt_install() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    for (int i = 0; i < 256; i++) {
        idt_entries[i].base_lo = 0;
        idt_entries[i].base_hi = 0;
        idt_entries[i].sel = 0;
        idt_entries[i].always0 = 0;
        idt_entries[i].flags = 0;
    }

    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}
