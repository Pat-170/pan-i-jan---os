#include "pmm.h"
#include "mmap.h"
#include "vga.h"

#define PAGE_SIZE 4096

static uint32_t mem_top = 0;          // najwyższy fizyczny adres +1
static uint32_t frames_count = 0;
static uint32_t frames_used  = 0;

static uint32_t* bitmap = 0;          // 1 bit per frame
static uint32_t  bitmap_len_u32 = 0;

static inline void bset(uint32_t i){ bitmap[i>>5] |=  (1u<<(i&31)); }
static inline void bclr(uint32_t i){ bitmap[i>>5] &= ~(1u<<(i&31)); }
static inline int  bget(uint32_t i){ return (bitmap[i>>5] >> (i&31)) & 1u; }

static void mark_region(uint64_t base, uint64_t len, int used){
    if (frames_count == 0 || bitmap == 0) return; // nic jeszcze nie ma
    uint64_t start = (base + PAGE_SIZE-1) & ~(uint64_t)(PAGE_SIZE-1);
    uint64_t end   = (base + len) & ~(uint64_t)(PAGE_SIZE-1);
    for (uint64_t a = start; a < end; a += PAGE_SIZE){
        uint32_t f = (uint32_t)(a / PAGE_SIZE);
        if (f < frames_count){
            if (used){ if (!bget(f)) { bset(f); frames_used++; } }
            else     { if ( bget(f)) { bclr(f); frames_used--; } }
        }
    }
}

void pmm_init(const multiboot_info_t* mbi, uint32_t kstart, uint32_t kend){
    /* 1) wyznacz najwyższy adres z mapy (fallback: użyj sumarycznej wielkości) */
    mem_top = 0;
    if (mbi->flags & (1u<<6)) {
        uint32_t ptr = mbi->mmap_addr;
        uint32_t end = mbi->mmap_addr + mbi->mmap_length;
        while (ptr < end){
            multiboot_mmap_entry_t* e = (multiboot_mmap_entry_t*)ptr;
            uint64_t top = e->base_addr + e->length;
            if (top > mem_top) mem_top = (uint32_t)top;
            ptr += e->size + 4;
        }
    }
    if (mem_top == 0) {
        /* awaryjnie: użyj podsumowania z mmap.c */
        if (mmap_summary.total_bytes)
            mem_top = (uint32_t)mmap_summary.total_bytes;
        else
            mem_top = 16u * 1024u * 1024u; // minimalny fallback 16 MB
    }

    /* 2) liczba ramek + bitmapa za kernelem */
    frames_count = (mem_top + PAGE_SIZE-1) / PAGE_SIZE;
    frames_used  = frames_count;  // zaczynamy jako "wszystko zajęte"

    uint32_t bitmap_bytes = (frames_count + 7) / 8;
    bitmap_len_u32 = (bitmap_bytes + 3) / 4;
    bitmap = (uint32_t*)kend;

    for (uint32_t i=0;i<bitmap_len_u32;i++) bitmap[i] = 0xFFFFFFFF;

    uint32_t bitmap_phys_end = kend + bitmap_len_u32*4;

    /* 3) odznacz regiony typu 1 (wolne) */
    if (mbi->flags & (1u<<6)) {
        uint32_t ptr = mbi->mmap_addr;
        uint32_t end = mbi->mmap_addr + mbi->mmap_length;
        while (ptr < end){
            multiboot_mmap_entry_t* e = (multiboot_mmap_entry_t*)ptr;
            if (e->type == 1) mark_region(e->base_addr, e->length, 0);
            ptr += e->size + 4;
        }
    }

    /* 4) zarezerwuj 0..1MB i przestrzeń jądra + bitmapy */
    mark_region(0,        0x100000, 1);
    mark_region(kstart,   (uint64_t)bitmap_phys_end - kstart, 1);

    /* 5) log */
    vga_write("PMM: frames "); vga_write_dec(frames_count);
    vga_write(", used ");      vga_write_dec(frames_used);
    vga_write("\n");
}

void* pmm_alloc_frame(void){
    if (frames_count == 0 || bitmap == 0) return 0;
    for (uint32_t f=0; f<frames_count; ++f){
        if (!bget(f)){
            bset(f); frames_used++;
            return (void*)(f * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free_frame(void* phys){
    if (!phys) return;
    uint32_t f = (uint32_t)phys / PAGE_SIZE;
    if (f < frames_count && bget(f)){
        bclr(f); frames_used--;
    }
}

uint32_t pmm_total_frames(void){ return frames_count; }
uint32_t pmm_used_frames(void){  return frames_used;  }

