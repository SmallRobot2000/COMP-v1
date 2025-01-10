        bits 16
        cpu 8086
        org     0100h
        global	putmchar
        global  _start
        extern  main
; INT 06h: print character in AL
%macro PUTCM 0
        int     06h
%endmacro

;Clobbers: ch
%macro gameReloadWrite 0
        mov     ch, 5
        ;int     09h ;game interupt
%endmacro


_start:

    call    main

    retf
putmchar:
    mov     ax,[bp-2]
    PUTCM
    ret


