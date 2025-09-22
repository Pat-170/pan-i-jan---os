.intel_syntax noprefix
.code32

/* ----- IDT loader ----- */
.global idt_load
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret

/* ----- IRQ0: PIT ----- */
.section .text
.global irq0
.extern  pit_handler
irq0:
    pusha
    call pit_handler
    popa
    iretd

/* ----- IRQ1: Keyboard ----- */
.global irq1
.extern  keyboard_handler
irq1:
    pusha
    call keyboard_handler
    popa
    iretd

.section .note.GNU-stack,"",@progbits

