section .data
    gdt_start:
        db 0, 0, 0, 0, 0, 0, 0, 0      
        dw 0xFFFF, 0x0000, 0x9A, 0xCF   
        dw 0xFFFF, 0x0000, 0x92, 0xCF   

    gdt_end:

    gdt_ptr:
        dw gdt_end - gdt_start - 1     
        dd gdt_start                  

section .text
    global gdt_install

    

gdt_install:
    call enable_a20

    lgdt [gdt_ptr]              

    mov eax, cr0
    or eax, 1
    mov cr0, eax                  

    jmp protected_mode_entry

enable_a20:
    cli       

    in al, 0x64                    
    test al, 0x02

    jnz wait_for_kbc_ready      
    jmp enable_a20              

wait_for_kbc_ready:
    mov al, 0xD0  

    out 0x64, al               
    in al, 0x60                    
    test al, 0x01

    jz send_a20_command         

send_a20_command:
    mov al, 0xDF                
    out 0x64, al               
    sti                         

    ret                         


protected_mode_entry:
    sti     
    hlt                      
    jmp $                         
