#pragma once
#include <stdint.h>
#include "multiboot.h"

typedef struct {
    uint64_t total_bytes;
    uint64_t usable_bytes;
} mmap_summary_t;

extern mmap_summary_t mmap_summary;

void mmap_init(const multiboot_info_t* mbi);
void mmap_dump(void); // opcjonalne wypisanie do VGA
