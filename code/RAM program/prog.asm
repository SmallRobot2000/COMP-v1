        cpu 8086
        org 0100h
%include"../api.inc"
start:
        mov     al,'U'
        PUTC
        jmp     start
        retf


