#include "mmap.h"
#include "vga.h"

mmap_summary_t mmap_summary = {0,0};

void mmap_init(const multiboot_info_t* mbi){
    mmap_summary.total_bytes = 0;
    mmap_summary.usable_bytes = 0;

    if (!(mbi->flags & (1u<<6))) {
        vga_write("No multiboot mmap\n");
        return;
    }

    uint32_t ptr = mbi->mmap_addr;
    uint32_t end = mbi->mmap_addr + mbi->mmap_length;

    while (ptr < end){
        multiboot_mmap_entry_t* e = (multiboot_mmap_entry_t*)ptr;
        mmap_summary.total_bytes += e->length;
        if (e->type == 1) mmap_summary.usable_bytes += e->length;
        ptr += e->size + 4;
    }
}

void mmap_dump(void){
    vga_write("RAM total: ");  vga_write_dec((unsigned long)(mmap_summary.total_bytes / (1024*1024))); vga_write(" MB  ");
    vga_write("usable: ");     vga_write_dec((unsigned long)(mmap_summary.usable_bytes / (1024*1024))); vga_write(" MB\n");
}
