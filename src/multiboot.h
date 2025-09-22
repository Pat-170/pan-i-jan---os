#pragma once
#include <stdint.h>

#define MULTIBOOT_MAGIC      0x2BADB002

typedef struct {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;     // 1 = available
} __attribute__((packed)) multiboot_mmap_entry_t;

typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t boot_device;
    uint32_t cmdline;

    uint32_t mods_count;
    uint32_t mods_addr;

    uint32_t syms[4];

    uint32_t mmap_length;
    uint32_t mmap_addr;

    // reszta pól nas nie interesuje na razie
} __attribute__((packed)) multiboot_info_t;

typedef struct {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} __attribute__((packed)) multiboot_module_t;
