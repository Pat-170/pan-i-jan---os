ENTRY(_start)

SECTIONS
{
  /* adres ładowania w pamięci (1 MiB) */
  . = 1M;

  /* Tekst: NAJPIERW nagłówek multiboot, potem kod i rodata. */
  .text ALIGN(4K) : {
    KEEP(*(.multiboot))     /* <- musi być absolutnie pierwsze w pliku */
    *(.text*)
    *(.rodata*)
  }

  .data ALIGN(4K) : { *(.data*) }

  .bss  ALIGN(4K) : { *(COMMON) *(.bss*) }
}

