#include <stdint.h>
#include "vga.h"

#define VGA_MEM   ((volatile uint16_t*)0xB8000)
#define VGA_COLS  80
#define VGA_ROWS  25
#define VGA_COLOR 0x07  /* light gray on black */

static int cur_row = 0, cur_col = 0;

static inline void outb(uint16_t p, uint8_t v){
    __asm__ __volatile__("outb %b0,%w1"::"a"(v),"Nd"(p));
}

static void vga_hw_cursor(int row, int col){
    int pos = row * VGA_COLS + col;
    outb(0x3D4, 0x0F); outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E); outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static inline void vga_putc_at_raw(int row, int col, char c, uint8_t color){
    VGA_MEM[row*VGA_COLS + col] = ((uint16_t)color << 8) | (uint8_t)c;
}

static void vga_scroll_if_needed(void){
    if (cur_row < VGA_ROWS) return;
    for (int r=1; r<VGA_ROWS; ++r){
        for (int c=0; c<VGA_COLS; ++c){
            VGA_MEM[(r-1)*VGA_COLS + c] = VGA_MEM[r*VGA_COLS + c];
        }
    }
    for (int c=0; c<VGA_COLS; ++c) vga_putc_at_raw(VGA_ROWS-1, c, ' ', VGA_COLOR);
    cur_row = VGA_ROWS-1;
}

void vga_init(void){
    vga_clear();
}

void vga_clear(void){
    for (int r=0; r<VGA_ROWS; ++r)
        for (int c=0; c<VGA_COLS; ++c)
            vga_putc_at_raw(r, c, ' ', VGA_COLOR);
    cur_row = 0; cur_col = 0;
    vga_hw_cursor(cur_row, cur_col);
}

void vga_putc(char c){
    if (c == '\n'){
        cur_col = 0;
        cur_row++;
    } else if (c == '\r'){
        cur_col = 0;
    } else if (c == '\b'){
        if (cur_col > 0){
            cur_col--;
            vga_putc_at_raw(cur_row, cur_col, ' ', VGA_COLOR);
        } else if (cur_row > 0){
            cur_row--;
            cur_col = VGA_COLS-1;
            vga_putc_at_raw(cur_row, cur_col, ' ', VGA_COLOR);
        }
    } else {
        vga_putc_at_raw(cur_row, cur_col, c, VGA_COLOR);
        cur_col++;
        if (cur_col >= VGA_COLS){ cur_col = 0; cur_row++; }
    }
    vga_scroll_if_needed();
    vga_hw_cursor(cur_row, cur_col);
}

void vga_write(const char *s){
    while (*s) vga_putc(*s++);
}

void vga_write_dec(unsigned long n){
    if (n == 0){ vga_putc('0'); return; }
    char buf[21]; int i=0;
    while (n){ buf[i++] = '0' + (n % 10); n /= 10; }
    while (i--) vga_putc(buf[i]);
}

