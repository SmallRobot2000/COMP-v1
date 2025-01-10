

SOH             equ     0x01  ; Start of Header
EOT             equ     0x04  ; End of Transmission
ACK             equ     0x06  ; Acknowledge
NAK             equ     0x15  ; Negative Acknowledge
CAN             equ     0x18  ; Cancel
BLOCK_SIZE      equ     128


;es - destination segment
;bx - destination offset
; Clobbers: AX, BX, DX, CX, DI, ES
;call    uart_getch
;call    uart_outch
XMODEM_RECEVE:
    mov     al,NAK
    call    uart_outch
    mov     di,0    ;set data start to 0
    
    
    
x_pac_get:
    call    uart_getch
.XnewPac:
    cmp     al,SOH          ;See if we get a SOH if not end transmission
    jz      .xmod_ok1
.xPacGone:                  ;Packet gone
    cmp     al,EOT
    jz      .Xend
    mov     al,CAN
    call    uart_outch

    mov     dl,XM_DATA
    mov     ax,0x0F00+'X'
    call    xo_write_reg
    ret                     ;oh ne
.xmod_ok1:                   ;Packet OK
    mov     ax,0x0F00+'K'   ;Debug ok
    call    xo_write_reg


    call    uart_getch

    mov     dh,al           ;packet number
    call    uart_getch
                           ;inverse packet number
    and     dh,al
    cmp     dh,0
    jnz     .xPacGoneN
    mov     cx,128
    mov     dh,0            ;dh will be packet cheksum

.xloop:                     ;Data loop
    call    uart_getch      ;Get data byte
    mov     [es:bx+di],al



    clc
    add     dh,al

    clc
    inc     di
    cmp     di,0            ;See if we passed segment border
    jnz      .XnotNewSeg
    mov     ax,es
    inc     ax
    mov     es,ax

.XnotNewSeg:
    dec     cx
    cmp     cx,0
    jz      .XendDat
    jmp     .xloop  ;go back to data loop

.XendDat:                   ;End of a packet
    

    call    uart_getch      ;get cheksum


    cmp     al,dh
    jnz     .xPacGoneC       ;Cheksumm error
    mov     al,ACK
    call    uart_outch
    call    uart_getch
    cmp     al,CAN
    jz      .Xend

    
    jmp     .XnewPac
    
.Xend:

    ;call    dump
    mov     al,NAK
    call    uart_outch
    call    uart_getch
    cmp     al,EOT
    mov     al,ACK
    call    uart_outch
    
    mov     dl,XM_DATA
    mov     ax,0x0F00+'E'
    call    xo_write_reg
    ret



.xPacGoneC:
    mov     al,CAN
    call    uart_outch
    mov     dl,XM_XDATA
    mov     ax,0x0F00+'C'
    call    xo_write_reg
    ret

.xPacGoneN:
    mov     al,CAN
    call    uart_outch
    mov     dl,XM_XDATA
    mov     ax,0x0F00+'N'
    call    xo_write_reg
    ret


dump:
    mov     dl,XM_DATA
    mov     di,0
    mov     cx,128*2
.loop:
    mov     al,[es:bx+di]
    call    xo_write_reg
    inc     di
    dec     cx
    cmp     cx,0
    jz      .loopend
    jmp .loop
.loopend:
    ret