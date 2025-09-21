#include <stdint.h>
#include "vga.h"

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
static uint8_t row=0, col=0; 
static uint8_t attr=0x07; // szary na czarnym

static void vga_newline(){
  col=0;
  if(++row>=25){
    row=24;
    for(int r=0;r<24;++r)
      for(int c=0;c<80;++c)
        VGA[r*80+c]=VGA[(r+1)*80+c];
    for(int c=0;c<80;++c)
      VGA[24*80+c]=(attr<<8)|' ';
  }
}

void vga_init(void){ vga_clear(); }
void vga_clear(void){
  for(int i=0;i<80*25;i++) VGA[i]=(attr<<8)|' ';
  row=col=0;
}
void vga_putc(char c){
  if(c=='\n'){ vga_newline(); return; }
  if(c=='\r'){ col=0; return; }
  if(c=='\b'){ if(col) col--; VGA[row*80+col]=(attr<<8)|' '; return; }
  VGA[row*80+col]=(attr<<8)|c;
  if(++col>=80) vga_newline();
}
void vga_write(const char* s){ while(*s) vga_putc(*s++); }
