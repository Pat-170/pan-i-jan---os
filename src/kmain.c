#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"

/* mini utils – bez libc */
static int streq(const char* a, const char* b){
    while (*a && *b){ if (*a != *b) return 0; ++a; ++b; }
    return *a == *b;
}
static int starts_with(const char* s, const char* p){
    while (*p){ if (*s++ != *p++) return 0; } return 1;
}

void kmain(void){
    __asm__ __volatile__("cli");

    vga_init();
    vga_write("Minimal OS: boot OK\n");

    gdt_init();  vga_write("GDT ready\n");
    idt_init();  vga_write("IDT ready\n");

    pic_remap(0x20, 0x28);
    pic_mask_all();

    idt_install_irqs();        // 32 -> PIT, 33 -> Keyboard (masz to w idt.c)

    pit_init(100);
    irq_unmask(0);             // PIT
    keyboard_init();           // odmaskuje IRQ1 i wlaczy skanowanie

    __asm__ __volatile__("sti");

    /* prompt + bufor linii */
    char line[80]; int len = 0;
    vga_write("> ");

    unsigned long last = (unsigned long)-1;
    for(;;){
        /* co sekundę pokazuj uptime */
        unsigned long s = pit_seconds();
        if (s != last){
            last = s;
            vga_write("\nuptime: ");
            vga_write_dec(s);
            vga_write("s  kb_irqs=");
            vga_write_dec(kb_irq_count());
            vga_write("\n> ");
            /* po statusie odrysuj to co już wpisane w linii */
            for (int i=0;i<len;i++) { extern void vga_putc(char); vga_putc(line[i]); }
        }

        /* non-blocking echo z klawiatury + obsluga backspace/enter */
        while (kb_have()){
            extern void vga_putc(char);
            char c = kb_get();

            if (c == '\b'){               /* Backspace */
                if (len > 0){
                    len--;
                    vga_putc('\b');       /* zakladamy, ze Twoje vga_putc umie cofnąć i skasować */
                }
            } else if (c == '\n'){        /* Enter -> wykonaj polecenie */
                line[len] = '\0';
                vga_putc('\n');

                if (streq(line, "help")){
                    vga_write("cmds: help | uptime | echo <txt>\n");
                } else if (streq(line, "uptime")){
                    unsigned long us = pit_seconds();
                    vga_write("uptime: "); vga_write_dec(us); vga_write("s\n");
                } else if (starts_with(line, "echo ")){
                    vga_write(line + 5); vga_putc('\n');
                } else if (len == 0){
                    /* no-op */
                } else {
                    vga_write("unknown cmd: "); vga_write(line); vga_putc('\n');
                }

                /* nowa linia wejściowa */
                len = 0;
                vga_write("> ");
            } else {
                if (len < (int)sizeof(line)-1){
                    line[len++] = c;
                    vga_putc(c);          /* echo */
                }
            }
        }

        __asm__ __volatile__("hlt");
    }
}

