#pragma once
#include <stdint.h>
#include "multiboot.h"

void  pmm_init(const multiboot_info_t* mbi, uint32_t kernel_start, uint32_t kernel_end);
void* pmm_alloc_frame(void);   // zwraca FIZYCZNY adres początku strony 4KiB
void  pmm_free_frame(void* phys);
uint32_t pmm_total_frames(void);
uint32_t pmm_used_frames(void);
