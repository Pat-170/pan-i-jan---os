#include <stdint.h>
#include "keyboard.h"
#include "pic.h"

/* I/O helpers */
static inline uint8_t inb(uint16_t p){ uint8_t r; __asm__ __volatile__("inb %w1,%0":"=a"(r):"Nd"(p)); return r; }
static inline void    outb(uint16_t p, uint8_t v){ __asm__ __volatile__("outb %b0,%w1"::"a"(v),"Nd"(p)); }
static inline void    io_wait(void){ __asm__ __volatile__("outb %b0,%w1"::"a"(0),"Nd"(0x80)); }

#define KBD_DATA 0x60
#define KBD_STAT 0x64
#define KBD_CMD  0x64

/* ring buffer */
#define KBBUF_SZ 128
static volatile char kbbuf[KBBUF_SZ];
static volatile unsigned head=0, tail=0;

static inline void kb_enqueue(char c){
    unsigned n = (head + 1) % KBBUF_SZ;
    if (n != tail){ kbbuf[head] = c; head = n; }
}
int kb_have(void){ return head != tail; }
char kb_get(void){
    if (head == tail) return 0;
    char c = kbbuf[tail];
    tail = (tail + 1) % KBBUF_SZ;
    return c;
}

/* debug */
static volatile unsigned kb_irqs = 0;
unsigned kb_irq_count(void){ return kb_irqs; }

/* mapy skan->ASCII (US) */
static const char map_norm[128] = {
    /*0x00*/ 0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    /*0x10*/ '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    /*0x20*/ 'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x',
    /*0x30*/ 'c','v','b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,
};
static const char map_shift[128] = {
    /*0x00*/ 0,27,'!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    /*0x10*/ '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
    /*0x20*/ 'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X',
    /*0x30*/ 'C','V','B','N','M','<','>','?',0,'*',0,' ',0,0,0,0,
};

static inline void kbd_wait_write(void){ while (inb(KBD_STAT) & 0x02) { } }
static inline int  kbd_can_read(void){  return (inb(KBD_STAT) & 0x01) != 0; }

static volatile int shift = 0;

void keyboard_init(void){
    /* włącz interfejs i skanowanie (nie każdy BIOS to robi) */
    kbd_wait_write(); outb(KBD_CMD, 0xAE); io_wait();  /* enable keyboard interface */
    kbd_wait_write(); outb(KBD_DATA, 0xF4); io_wait(); /* enable scanning */
    irq_unmask(1);
}

void keyboard_handler(void){
    kb_irqs++;

    if (kbd_can_read()){
        uint8_t sc = inb(KBD_DATA);

        if (sc == 0x2A || sc == 0x36) shift = 1;       /* LSHIFT/RSHIFT make */
        else if (sc == 0xAA || sc == 0xB6) shift = 0;  /* LSHIFT/RSHIFT break */
        else if (!(sc & 0x80)){                        /* make codes */
            char c = (shift ? map_shift : map_norm)[sc & 0x7F];
            if (c) kb_enqueue(c);
        }
    }
    pic_send_eoi(1);
}

