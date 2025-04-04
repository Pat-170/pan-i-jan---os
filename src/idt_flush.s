    bits 32

global idt_flush
idt_flush:
    mov eax, [esp + 4]   ; Load pointer to IDT descriptor into eax
    lidt [eax]           ; Load the IDT
    ret
