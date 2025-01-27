%include "gameduino.inc"

program:

        mov     bx, 2800h
        gameUpdateRead
        gameRead
        
        mov     si, MY_FSTR
        push    ax
        gameReloadWrite
        PRINTF

        mov     si, LAODSTR
        PUTS

        mov     al,'R'  ;MASTER RESET
        call    ARD_SPI_COMMAND
        nop
        nop
        nop
.waitRedy:
        mov     al,'S'  ;Get number of blocks and reset
        call    ARD_SPI_COMMAND
        cmp     al,'R'
        jnz     .waitRedy

        mov     al,'B'  ;Get number of blocks and reset
        call    ARD_SPI_COMMAND

        ;push    ax
        ;gameReloadWrite
        ;pop     ax
        mov     [TMP_VAR],al    ;Save num of blocks
        ;call    print_hexbyte
        ;mov     ax, 0Ah         ;NL
        ;PUTC

        clr     di
        mov     al,'T'
        call    ARD_SPI_COMMAND ;Get segment High byte in bh
        mov     bh,al
        mov     al,'t'  
        call    ARD_SPI_COMMAND ;Get segment Low byte   
        mov     ah,bh   ;GEt full segment in AX to ES
        mov     es,ax   ;Set segment in es
        
        ;gameReloadWrite
        ;push    es
        ;mov     si, SEGMENTSTR
        ;PRINTF

        mov     al,'O'
        call    ARD_SPI_COMMAND ;Get offset High byte
        mov     bh,al
        mov     al,'o'  
        call    ARD_SPI_COMMAND ;Get offset Low byte
        mov     bl,al           ;Get offset into bx

        ;gameReloadWrite
        ;push    bx
        ;mov     si, OFFSETSTR
        ;PRINTF
        

        ;Counters
        clr     di              ;count to FFFFh
        clr     dx              ;count to 1000h
loping:
        mov     al,'D'
        call    ARD_SPI_COMMAND
        mov     [es:bx+di], al
        cmp     di, 0FFFFh      ;End of 16bit reg so need to increment segment
        jz      newSeg
        inc     di
newSeg_ret:
        
        inc     dx              ;count in block

        ;gameReloadWrite
        ;push    dx
        ;mov     si,DATASTR
        ;PRINTF
        ;mov     ax,[GAME_ADD_W]
        ;sub     ax,6
        ;mov     [GAME_ADD_W],ax
        ;gameReloadWrite

        cmp     dx, SPI_BLOCK_SIZE       ;End of the block so neet to chek for new block
        jz      chk_end

        jmp     loping
newSeg:
        gameReloadWrite
        mov     si, NEWSEGSTR
        PUTS
        push    ax
        mov     ax,es
        add     ax,1000h      ;incerment ES
        mov     es,ax
        pop     ax
        clr     di     ;dont clear it because it will wrap araund
        jmp     newSeg_ret

chk_end:
        gameReloadWrite
        mov     al,'#'
        PUTC
        clr     dx
chk_end_loop:
        mov     al,'S'
        call    ARD_SPI_COMMAND
        cmp     al, 'R'
        nop
        jz      loping  ;DATA ready
        cmp     al, 'E'
        jz      end     ;End of data
        jmp     chk_end_loop    ;else data not ready
end:
        

        gameReloadWrite
        mov     si,ENDISSTR
        PUTS


        mov     ax, 0Ah
        PUTC
        ;mov     al,[TMP_VAR]
        ;clr     ah
        ;mov     cx,SPI_BLOCK_SIZE
        ;mul     cx      ;(SPI_BLOCK_SIZE * num of blocks) result in DX and AX
;
        ;push    dx
        ;push    ax      ;for printf
        ;mov     si, DATA_END
        ;PRINTF
        ;jmp     $       ;Just wait til infinity
;.printStrloop:
;        mov     al,[DATA_BUFFER+si]
;        cmp     al, 0
;        jz     endS
;        PUTC
;        inc     si
;        jmp     .printStrloop
;endS:
;        jmp     $
        ;far call to AX:BX
        clr     di
        mov     al,'T'
        call    ARD_SPI_COMMAND ;Get segment High byte in bh
        mov     bh,al
        mov     al,'t'  
        call    ARD_SPI_COMMAND ;Get segment Low byte   
        mov     ah,bh   ;GEt full segment in AX to ES
        mov     es,ax   ;Set segment in es


        mov     al,'O'
        call    ARD_SPI_COMMAND ;Get offset High byte
        mov     bh,al
        mov     al,'o'  
        call    ARD_SPI_COMMAND ;Get offset Low byte
        mov     bl,al           ;Get offset into bx
        
        mov     ax,0100h
        mov     es,ax
        clr     bx

        push es
        push bx
        mov  bp, sp
        call FAR [bp]
        add  sp, 4
        gameReloadWrite
        mov     si,CALL_RETF
        PUTS
        jmp     $
ARD_SPI_COMMAND:
        push    ax
        mov     cl,0FFh
        SPIselect
        mov     cl, 1
        SPIselect
        pop     ax
        SPItransferByte
        nop
        SPItransferByte
        mov     cl,0FFh
        SPIselect
        ret


MY_FSTR:
        db      'Gameduino ID - ', FMT_H8, 0Ah,0
MY_FSTR2:
        db      'SPI got - ', FMT_H16, 0Ah,0
MY_STR3:
        db      'Blocks: ',0
CALL_RETF:
        db      0Ah, 'Call returned!', 0
DATA_END:
        db      'End of data! size:',FMT_H32, 0
SEGMENTSTR:
        db      0Ah,'Segment: ',FMT_H16, 0
OFFSETSTR:
        db      0Ah,'Offset: ',FMT_H16, 0
ENDCHKSTR:
        db      0Ah,'Cheking for end!', 0
ENDISSTR:
        db      0Ah,'Done', 0
DATASTR:
        db      'D:',FMT_H16,0
LAODSTR:        
        db      'Loading...',0Ah,0
NEWSEGSTR:
        db      'New segment!',0Ah,0