%include "testAPI.inc"
%include "API.inc"
%include "ram_vars_define.inc"
        cpu 8086
        org 0000h
        gameReloadWrite
        mov     si,testSTR
        mov     ax,cs
        mov     es,ax
        call    puts
        retf




puts:
        push    ax
        push    ds
        mov     ax, es
        mov     ds, ax
.1:     lodsb           ; get character
        or      al, al  ; end if 0
        jz      .done
        PUTC
        jmp     .1
.done:  pop     ds
        pop     ax
        ret

testSTR:
        db      "Jel ovo moguce da radi?????",0

