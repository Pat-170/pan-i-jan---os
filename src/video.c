// VGA text mode memory starts at 0xB8000 <--- memorize it ~jakub
#define VIDEO_MEMORY ((uint16_t*) 0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef unsigned short uint16_t;

#include "/home/kusn/code/kernel/headers/kernel.h"


void clear_screen() {
    uint16_t blank = 0x0F00; // WHITE on BLACK 

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VIDEO_MEMORY[i] = blank;
    }
}

void print_string(const char* str) {
    uint16_t* screen = VIDEO_MEMORY;
    
    while (*str) {
        *screen = (0x0F << 8) | *str;
        screen++;
        str++;
    }
}