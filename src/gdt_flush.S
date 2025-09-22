.intel_syntax noprefix
.code32

.global gdt_flush
gdt_flush:
    mov eax, [esp+4]         # &gdt_ptr
    lgdt [eax]

    mov ax, [esp+8]          # code_sel (0x08)
    push eax
    push offset 1f
    retf                     # daleki skok przeładuje CS
1:
    mov ax, [esp+12]         # data_sel (0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
