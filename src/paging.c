#include "paging.h"
#include "pmm.h"
#include "vga.h"

#define PAGE_SIZE 4096
#define PD_ENTRIES 1024
#define PT_ENTRIES 1024
#define PRESENT 0x001
#define WRITE   0x002
#define USER    0x004

static uint32_t* page_directory = 0;

extern uint32_t kernel_end; // z linker.ld

static uint8_t* heap_ptr = 0;

void paging_init_identity_64mb(void){
    /* sprawdź czy PMM żyje */
    if (pmm_total_frames() == 0){
        vga_write("Paging: PMM not ready (0 frames)\n");
        return;
    }

    page_directory = (uint32_t*)pmm_alloc_frame();
    if (!page_directory){
        vga_write("Paging: no frame for PD\n");
        return;
    }
    for (int i=0;i<PD_ENTRIES;i++) page_directory[i] = 0;

    for (uint32_t pd_i=0; pd_i<16; ++pd_i){ // 16*4MB = 64MB
        uint32_t* pt = (uint32_t*)pmm_alloc_frame();
        if (!pt){
            vga_write("Paging: no frame for PT\n");
            return;
        }
        for (int j=0;j<PT_ENTRIES;j++) pt[j] = 0;

        for (uint32_t pt_i=0; pt_i<PT_ENTRIES; ++pt_i){
            uint32_t addr = (pd_i*0x400000u) + pt_i*PAGE_SIZE;
            pt[pt_i] = (addr & ~0xFFFu) | PRESENT | WRITE;
        }
        page_directory[pd_i] = ((uint32_t)pt) | PRESENT | WRITE;
    }

    /* Załaduj CR3 + włącz PG */
    __asm__ __volatile__("mov %0, %%cr3"::"r"(page_directory));
    uint32_t cr0; __asm__ __volatile__("mov %%cr0, %0":"=r"(cr0));
    cr0 |= 0x80000000u; // PG
    __asm__ __volatile__("mov %0, %%cr0"::"r"(cr0));

    vga_write("Paging: identity 64MB ON\n");

    /* prosty heap za końcem kernela */
    heap_ptr = (uint8_t*)(((uint32_t)&kernel_end + 15) & ~15u);
}

void* kmalloc(uint32_t size){
    if (size == 0) return 0;
    size = (size + 15) & ~15u;
    void* p = heap_ptr;
    heap_ptr += size;
    return p; // bez free
}

