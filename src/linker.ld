ENTRY(_start)

SECTIONS
{
    . = 1M; /* Load kernel at 1MB */

    .text : ALIGN(4K)
    {
        *(.multiboot) /* Multiboot header */
        *(.text) /* Code */
    }

    .data : ALIGN(4K) { *(.data) }
    .bss : ALIGN(4K) { *(.bss) }
}
