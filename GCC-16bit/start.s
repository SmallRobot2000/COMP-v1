.section .text
.code16 /* Ensure 16-bit mode */
.global start

start:
    cli
    mov $0x1000, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $0x7C00, %sp

    call main

hang:
    hlt
    jmp hang
