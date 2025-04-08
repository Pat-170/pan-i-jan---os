
section .text
    global protmodesetup
    extern _start

protmodesetup:
    cli

    ; Enable A20 line here
    in al, 0x92
    or al, 2
    out 0x92, al

    xor ax, ax
    mov ds, ax           ; Set DS to 0 for GDT setup

    lgdt [GDT_DESC]      ; Load the GDT

    ; Set CR0 to enable Protected Mode
    mov eax, 0x1
    mov cr0, eax

    jmp GDT_kernelCodeSegment-GDT:protmode   

[BITS 32]

protmode:
    mov ax, GDT_kernelDataSegment - GDT  
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000  
    
    jmp _start


                
align 4
GDT_DESC:
    dw GDT_END - GDT - 1           ; GDT size - 1
    dd GDT                         ; GDT pointer

align 16
GDT:
    GDT_NULL: dq 0x0000000000000000  ; Null descriptor
    GDT_kernelCodeSegment: dq 0x00CF9A000000FFFF  ; Kernel Code Segment
    GDT_kernelDataSegment: dq 0x00CF92000000FFFF  ; Kernel Data Segment
    GDT_userCodeSegment: dq 0x00CFFA000000FFFF    ; User Code Segment
    GDT_userDataSegment: dq 0x00CF92000000FFFF    ; User Data Segment
    GDT_TSM: dq 0x0000000000000000               ; Task State Management
    GDT_CS64: dq 0x00209A0000000000              ; 64-bit CS

GDT_END: