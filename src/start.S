.intel_syntax noprefix
.code32

/* Punkt wejścia dla linker.ld: _start */
.global _start
.extern kmain

_start:
    cli
    /* ustaw 16-KB stosu w .bss */
    mov esp, offset stack_top
    call kmain

.hang:
    hlt
    jmp .hang

/* --- prosty stos w .bss --- */
.section .bss
.align 16
stack_bottom:
    .skip 16384            /* 16 KiB */
stack_top:
