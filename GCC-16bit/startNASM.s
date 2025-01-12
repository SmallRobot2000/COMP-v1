   bits 16     ; Ensure 16-bit mode
   CPU 8086
   global start
   extern main
   

    EXTERN __dataoffset
        EXTERN __ldata
        EXTERN __sbss
        EXTERN __lbss
start:
    push    ax
    push    bx
    push    cx
    push    dx
    push    ds
    push    es



    mov bx, ss
    mov cx, sp
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0xFFF0
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
