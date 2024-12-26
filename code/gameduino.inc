%include "ram_vars_define.inc"
GAME_IOBASE     equ     80h
GAME_DATA       equ     GAME_IOBASE
GAME_SS         equ     GAME_IOBASE+1
GAME_SS_MASK    equ     00000001b

SPI_DATA        equ     GAME_DATA
SPI_SS          equ     GAME_SS

SPI_SS_MASK_0   equ     00000001b   
SPI_SS_MASK_1   equ     00000010b
SPI_SS_MASK_2   equ     00000100b
SPI_SS_MASK_3   equ     00001000b
SPI_SS_MASK_4   equ     00010000b
SPI_SS_MASK_5   equ     00100000b
SPI_SS_MASK_6   equ     01000000b
SPI_SS_MASK_7   equ     10000000b


; Send byte in AL to GAME address in BX. And sets all other SSs to 1
; Clobbers: AH and BX
game_init:
        push    ds
        mov     ax, RAM_VARS_SEGMENT
        mov     ds, ax
        mov     al, 0FFh
        mov     [GAME_SS_BUF], al   ;set SS status
        mov     bx, 0
        mov     [GAME_ADD_W], bx ;set add
        call    game_set_add_write
        pop     ds
        ret
game_sendByte:
        mov     ah, al
        mov     al, [GAME_SS_BUF]
        or      al, GAME_SS_MASK
        out     GAME_SS, al         ;deselect GAMEDUINO
        xor     al, GAME_SS_MASK
        out     GAME_SS, al         ;select GAMEDUINO
        or      bh, 80h
        mov     al, bh
        out     GAME_DATA, al
        nop
        mov     al, bl
        out     GAME_DATA, al
        nop
        mov     al, ah
        out     GAME_DATA, al
        mov     al, [GAME_SS_BUF]
        or      al, GAME_SS_MASK
        out     GAME_SS, al
        mov     [GAME_SS_BUF], al   ;save bew SS status
        mov     ax, [GAME_ADD_W]    ;increment game address word
        inc     ax
        mov     [GAME_ADD_W], ax
        ret


game_outch_int:
        push    ax
        push    ds
        push    bx

        mov     bx, ax
        mov     ax, RAM_VARS_SEGMENT
        mov     ds, ax  ;set corect data segment
        mov     ax, bx   ;get back original ax

        cmp     al,0Ah
        jz      .NL
        out     GAME_DATA, al
        mov     ax, [GAME_ADD_W]    ;increment game address word
        inc     ax
        mov     [GAME_ADD_W], ax

        pop     bx
        pop     ds
        pop     ax
        iret
.NL:
        mov     ax,[GAME_ADD_W]
        and     ax, 1111111111000000b
        add     ax, 64
        mov     bx, ax
        
        
        mov     [GAME_ADD_W], bx    ; update game add
        mov     al, [GAME_SS_BUF]
        or      al, GAME_SS_MASK
        out     GAME_SS, al         ;deselect GAMEDUINO
        xor     al, GAME_SS_MASK    ;select GAMEDUINO
        out     GAME_SS, al
        mov     [GAME_SS_BUF], al   ;save bew SS status
        or      bh, 80h
        mov     al, bh
        out     GAME_DATA, al
        nop
        mov     al, bl
        out     GAME_DATA, al

        pop     bx
        pop     ds
        pop     ax
        iret
; Set GAME address in BX for write
; Clobbers: AH and BX
game_set_add_write:
        mov     [GAME_ADD_W], BX    ; update game add
        mov     al, [GAME_SS_BUF]
        or      al, GAME_SS_MASK
        out     GAME_SS, al         ;deselect GAMEDUINO
        xor     al, GAME_SS_MASK    ;select GAMEDUINO
        out     GAME_SS, al
        mov     [GAME_SS_BUF], al   ;save bew SS status
        or      bh, 80h
        mov     al, bh
        out     GAME_DATA, al
        nop
        mov     al, bl
        out     GAME_DATA, al
        ret

; Set GAME address in BX for read.
; Clobbers: AH and BX
game_set_add_read:
        mov     [GAME_ADD_R], BX    ; update game add
        mov     al, [GAME_SS_BUF]
        or      al, GAME_SS_MASK
        out     GAME_SS, al         ;deselect GAMEDUINO
        xor     al, GAME_SS_MASK    ;select GAMEDUINO
        out     GAME_SS, al
        mov     [GAME_SS_BUF], al   ;save bew SS status
        mov     al, bh
        out     GAME_DATA, al
        nop
        nop
        mov     al, bl
        out     GAME_DATA, al
        ret

;game_readByte:
; read byte to AL, address is already set
; Clobbers: AH
game_readByte:
        out     GAME_DATA, al   ;what we send isnt important because we read
        mov     ax, [GAME_ADD_R]
        inc     ax
        mov     [GAME_ADD_R], ax        ;increment read address
        clr     ax
        in      al, GAME_DATA
        ret
;game_writeByte:
; write byte in AL, address is already set
; Clobbers: none
game_writeByte:
        push    ax
        mov     ax, [GAME_ADD_W]
        inc     ax
        mov     [GAME_ADD_W], ax        ;increment write address
        pop     ax
        out     GAME_DATA, al
        ret

;sets curent mode to write with address in GAME_ADD_W
; Clobbers: none
game_update_w:  
        push    bx
        push    ax
        mov     bx, [GAME_ADD_W]
        call    game_set_add_write
        pop     ax
        pop     bx
        ret

;sets curent mode to read with address in GAME_ADD_R
; Clobbers: none
game_update_r:  
        push    bx
        push    ax
        mov     bx, [GAME_ADD_W]
        call    game_set_add_read
        pop     ax
        pop     bx
        ret


;game_readByte (ch = 0)
; read byte to AL, address is already set
; Clobbers: none

;game_writeByte (ch = 1)
; write byte in AL, address is already set
; Clobbers: none

;game_update_r (ch = 2)
;sets curent mode to read with address in BX and updates GAME_ADD_R
; Clobbers: none

;game_update_w (ch = 3)
;sets curent mode to write with address in BX and updates GAME_ADD_W
; Clobbers: none

;game_reload_r (ch = 4)
;sets curent mode to read with address in GAME_ADD_R
; Clobbers: none

;game_reload_w (ch = 5)
;sets curent mode to write with address in GAME_ADD_W
; Clobbers: none

;(ch > 5) nothing

; vectord game I/O: ch determents the command

game_int:
        cmp     ch, 0
        jz      game_readByte_Int
        cmp     ch, 1
        jz      game_writeByte_Int
        cmp     ch, 2
        jz      game_update_r_Int
        cmp     ch, 3
        jz      game_update_w_Int
        cmp     ch, 4
        jz      game_reload_r_Int
        cmp     ch, 5
        jz      game_reload_w_Int
        iret

game_readByte_Int:
        push    ax
        out     GAME_DATA, al   ;what we send isnt important because we read
        mov     ax, [GAME_ADD_R]
        inc     ax
        mov     [GAME_ADD_R], ax        ;increment read address
        pop     ax
        in      al, GAME_DATA
        iret

game_writeByte_Int:
        push    ax
        mov     ax, [GAME_ADD_W]
        inc     ax
        mov     [GAME_ADD_W], ax        ;increment write address
        pop     ax
        out     GAME_DATA, al
        iret
game_update_r_Int:
        push    bx
        push    ax
        mov     [GAME_ADD_R], bx
        call    game_set_add_read
        pop     ax
        pop     bx
        iret
game_update_w_Int:
        push    bx
        push    ax
        mov     [GAME_ADD_W], bx
        call    game_set_add_write
        pop     ax
        pop     bx
        iret
game_reload_r_Int:
        push    bx
        push    ax
        mov     bx, [GAME_ADD_R]
        call    game_set_add_read
        pop     ax
        pop     bx
        iret
game_reload_w_Int:
        push    bx
        push    ax
        mov     bx, [GAME_ADD_W]
        call    game_set_add_write
        pop     ax
        pop     bx
        iret



;Clobbers: ch
%macro gameRead 0
        mov     ch, 0
        int     09h ;game interupt
%endmacro
;Clobbers: ch
%macro gameWrite 0
        mov     ch, 1
        int     09h ;game interupt
%endmacro
;Clobbers: ch
%macro gameUpdateRead 0
        mov     ch, 2
        int     09h ;game interupt
%endmacro
;Clobbers: ch
%macro gameUpdateWrite 0
        mov     ch, 3
        int     09h ;game interupt
%endmacro
;Clobbers: ch
%macro gameReloadRead 0
        mov     ch, 4
        int     09h ;game interupt
%endmacro
;Clobbers: ch
%macro gameReloadWrite 0
        mov     ch, 5
        int     09h ;game interupt
%endmacro

;SPI_transferByte (ch = 0)
; send byte from AL and receve byte to AL
; Clobbers: none

;SPI_select (ch = 1)
; select SPI device in CL (0 - 7) if CL > 7 then deselect all
; Clobbers: none

; vectord SPI functions for I/O: ch determents the command
SPI_transfer_int:
        cmp     ch, 0
        jz      SPI_transferByte_int
        cmp     ch, 1
        jz      SPI_select_int
        iret



SPI_transferByte_int:
        out     SPI_DATA, al
        nop
        nop
        nop
        nop
        nop
        in      al, SPI_DATA
        iret

SPI_select_int:
        cmp     CL, 0
        jz      SPI_select_int_0
        cmp     CL, 1
        jz      SPI_select_int_1
        cmp     CL, 2
        jz      SPI_select_int_2
        cmp     CL, 3
        jz      SPI_select_int_3
        cmp     CL, 4
        jz      SPI_select_int_4
        cmp     CL, 5
        jz      SPI_select_int_5
        cmp     CL, 6
        jz      SPI_select_int_6
        cmp     CL, 7
        jz      SPI_select_int_7
        push    ax
        mov     al, 0FFh
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al
        pop     ax
        iret

SPI_select_int_0:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_0
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_1:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_1
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_2:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_2
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_3:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_3
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_4:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_4
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_5:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_5
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_6:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_6
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
SPI_select_int_7:
        push    ax
        mov     al, [SPI_SS_BUF]
        xor     al, SPI_SS_MASK_7
        out     SPI_SS, al
        mov     [SPI_SS_BUF], al 
        pop     ax
        iret
;Clobbers: ch
%macro SPItransferByte 0
        mov     ch, 0
        int     0Ah ;game interupt
%endmacro
;Clobbers: ch
%macro SPIselect 0
        mov     ch, 1
        int     0Ah ;game interupt
%endmacro

%macro SPItransferByteFast 0
        out     SPI_DATA, al
        nop
        in      al, SPI_DATA
%endmacro