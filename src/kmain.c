#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"

#include "multiboot.h"
#include "mmap.h"
#include "pmm.h"
#include "paging.h"

/* --- utils bez libc --- */
static int streq(const char* a, const char* b){
    while (*a && *b){ if (*a != *b) return 0; ++a; ++b; }
    return *a == *b;
}
static int starts_with(const char* s, const char* p){
    while (*p){ if (*s++ != *p++) return 0; } return 1;
}

/* --- Pasek statusu (linia 0) --- */
#define VGA_MEM  ((volatile uint16_t*)0xB8000)
#define VGA_COLS 80
#define VGA_COLOR 0x07

static inline void vga_putc_at(int row, int col, char c, uint8_t color){
    VGA_MEM[row*VGA_COLS + col] = ((uint16_t)color << 8) | (uint8_t)c;
}
static void vga_write_at(int row, int col, const char* s, uint8_t color){
    while (*s && col < VGA_COLS){
        vga_putc_at(row, col++, *s++, color);
    }
}
static void vga_clear_line(int row){
    for (int c=0;c<VGA_COLS;c++) vga_putc_at(row,c,' ',VGA_COLOR);
}
static void write_dec_into(char* buf, unsigned long x, int* n){
    if (!x){ buf[(*n)++]='0'; return; }
    char t[21]; int i=0; while (x){ t[i++]='0'+(x%10); x/=10; }
    while (i--) buf[(*n)++]=t[i];
}

static int status_enabled = 1; /* pasek włączony domyślnie */
static void draw_status(unsigned long sec, unsigned kb_irqs){
    if (!status_enabled) return;
    char buf[80]; int n=0;
    const char* a="uptime: "; while (*a) buf[n++]=*a++;
    write_dec_into(buf, sec, &n);
    buf[n++]='s'; buf[n++]=' '; buf[n++]='|'; buf[n++]=' ';
    a="kb:"; while (*a) buf[n++]=*a++;
    write_dec_into(buf, kb_irqs, &n);
    while (n < VGA_COLS) buf[n++]=' ';
    vga_write_at(0,0,buf,VGA_COLOR);
}

/* symbole ustawiane w start.S */
extern uint32_t multiboot_magic;
extern multiboot_info_t* multiboot_info;

/* symbole z linker.ld */
extern uint32_t kernel_start, kernel_end;

void kmain(void){
    __asm__ __volatile__("cli");

    vga_init();
    vga_write("Minimal OS: boot OK\n");

    gdt_init();  vga_write("GDT ready\n");
    idt_init();  vga_write("IDT ready\n");

    /* ===== Memory map + PMM + Paging ===== */
    if (multiboot_magic != MULTIBOOT_MAGIC){
        vga_write("Bad multiboot magic!\n");
    } else {
        mmap_init(multiboot_info);
        mmap_dump();

        pmm_init(multiboot_info, (uint32_t)&kernel_start, (uint32_t)&kernel_end);

        if (pmm_total_frames() == 0) {
            vga_write("PMM not ready, skip paging\n");
        } else {
            paging_init_identity_64mb();
            void* p = kmalloc(128);
            vga_write("kmalloc test: "); vga_write_dec((unsigned long)(uint32_t)p); vga_putc('\n');
        }
    }

    /* ===== IRQ/PIC/PIT/KBD ===== */
    pic_remap(0x20, 0x28);
    pic_mask_all();

    idt_install_irqs();        /* 32 -> PIT, 33 -> Keyboard */

    pit_init(100);
    irq_unmask(0);             /* PIT */
    keyboard_init();           /* PS/2 + IRQ1 */

    __asm__ __volatile__("sti");

    char line[80]; int len = 0;
    vga_write("> ");

    unsigned long last_sec = (unsigned long)-1;

    for(;;){
        unsigned long s = pit_seconds();
        if (s != last_sec){
            last_sec = s;
            draw_status(s, kb_irq_count());
        }

        while (kb_have()){
            char c = kb_get();

            if (c == '\b'){
                if (len > 0){ len--; vga_putc('\b'); }
            } else if (c == '\n'){
                line[len] = '\0';
                vga_putc('\n');

                if (streq(line, "help")){
                    vga_write("cmds: help | uptime | echo <txt> | clear | status on|off | mem\n");
                } else if (streq(line, "uptime")){
                    vga_write("uptime: "); vga_write_dec(pit_seconds()); vga_write("s\n");
                } else if (starts_with(line, "echo ")){
                    vga_write(line+5); vga_putc('\n');
                } else if (streq(line, "clear")){
                    vga_clear();
                } else if (starts_with(line, "status ")){
                    if (streq(line+7,"on"))  { status_enabled = 1; draw_status(pit_seconds(), kb_irq_count()); }
                    if (streq(line+7,"off")) { status_enabled = 0; vga_clear_line(0); }
                } else if (streq(line, "mem")){
                    vga_write("frames used/total: ");
                    vga_write_dec(pmm_used_frames()); vga_write("/");
                    vga_write_dec(pmm_total_frames()); vga_putc('\n');
                } else if (len != 0){
                    vga_write("unknown cmd: "); vga_write(line); vga_putc('\n');
                }

                len = 0;
                vga_write("> ");
            } else {
                if (len < (int)sizeof(line)-1){
                    line[len++] = c;
                    vga_putc(c);
                }
            }
        }

        __asm__ __volatile__("hlt");
    }
}

