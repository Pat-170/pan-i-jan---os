#pragma once
#include <stdint.h>

void paging_init_identity_64mb(void);
void* kmalloc(uint32_t size); // prosty bump (bez free)
