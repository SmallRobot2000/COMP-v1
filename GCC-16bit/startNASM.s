   bits 16     ; Ensure 16-bit mode
   CPU 8086
   global start
   extern main
   
start:
    push    ax
    push    bx
    push    cx
    push    dx
    push    ds
    push    es

    mov bx, ss
    mov cx, sp
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ax, cs
    mov ss, ax
    mov sp, 0x7C00
    push    bx
    push    cx
    call    main
    pop     cx
    pop     bx
    mov     ss,bx
    mov     sp,cx
    pop     es
    pop     ds
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    retf



section .bss
    resb 512 - ($ - $$)  ; Fill the rest of the sector with zeros
