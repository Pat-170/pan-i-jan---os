#include <stdint.h>
#include "gdt.h"

/* Struktury GDT */
struct __attribute__((packed)) gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  gran;
    uint8_t  base_hi;
};

struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint32_t base;
};

extern void gdt_flush(uint32_t gdtp, uint16_t code_sel, uint16_t data_sel);

static struct gdt_entry gdt[3];
static struct gdt_ptr   gp;

static void gdt_set(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    gdt[i].limit_low = (limit & 0xFFFF);
    gdt[i].base_low  = (base  & 0xFFFF);
    gdt[i].base_mid  = (base  >> 16) & 0xFF;
    gdt[i].access    = access;
    gdt[i].gran      = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].base_hi   = (base  >> 24) & 0xFF;
}

void gdt_init(void){
    /* 0: NULL */
    gdt_set(0, 0, 0, 0, 0);

    /* 1: kernel code (0x08): base=0, limit=4GB, exec/read, ring0, present, 32-bit, granularity 4KB */
    gdt_set(1, 0, 0xFFFFF, 0x9A, 0xCF);

    /* 2: kernel data (0x10): base=0, limit=4GB, read/write, ring0, present, 32-bit, gran 4KB */
    gdt_set(2, 0, 0xFFFFF, 0x92, 0xCF);

    gp.limit = sizeof(gdt)-1;
    gp.base  = (uint32_t)&gdt[0];

    gdt_flush((uint32_t)&gp, GDT_CODE_SEL, GDT_DATA_SEL);
}
