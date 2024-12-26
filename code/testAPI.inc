;Clobbers: ch
%macro gameReloadWrite 0
        mov     ch, 5
        int     09h ;game interupt
%endmacro

