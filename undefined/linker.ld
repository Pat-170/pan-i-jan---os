ENTRY(_start)

SECTIONS
{
    . = 1M; /* Load kernel at 1MB */

    .multiboot : ALIGN(4K) 
    {
        *(.multiboot) /* This makes sure the multiboot header goes at the start */
    }

    .text : ALIGN(4K)
    {
        *(.text) /* Kernel code goes here */
    }

    .data : ALIGN(4K) 
    {
        *(.data) /* Data section (if any) */
    }
    
    .bss : ALIGN(4K) 
    {
        *(.bss) /* Uninitialized data section */
    }
}
