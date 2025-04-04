; keyboard_stub.s
global keyboard_handler_stub
extern keyboard_handler

section .text
keyboard_handler_stub:
    pusha                 ; Save all general-purpose registers
    call keyboard_handler ; Call your C keyboard handler
    popa                  ; Restore registers
    iretd                 ; Return from interrupt
