   bits 16     ; Ensure 16-bit mode
   CPU 8086
section .text
%include"asmDefines.inc"



global XoWriteReg, XoReadReg
global Keyb_INIT, Keyb_get_key, Keyb_key_pressed, Keyb_CAPS_on ,Keyb_CAPS_off, _KEY_PRESSED, _asm_reset   ;stais for asm


_asm_reset:
    jmp 0xF000:0xFFF0

XoWriteReg:

    mov     ax,ss
    mov     es,ax
    mov     bx, sp
    mov     dx, [es:bx + 2]   ; reg
    mov     ax, [es:bx + 4]   ; data

    

    mov     dh,0
    sal     dl,1
    add     dl,XOSERA_IOBASE
    and     dl,0FEh
    xchg    ah,al
    out     dx,ax

    ret





XoReadReg:
    mov     ax,ss
    mov     es,ax
    mov     bx, sp
    mov     dx, [es:bx + 2]   ; reg


    mov     dh,0
    sal     dl,1
    add     dl,XOSERA_IOBASE
    and     dl,0FEh
    or      dl,01h
    in      al,dx
    xchg    al,ah
    and     dl,0FEh
    in      al,dx
    xchg    al,ah       ;return in ax
    ret
_foo:
    
    ret
Keyb_get_key:
    mov     bx,BIOS_VARIABLES_OFFSET
    add     bx,BVAR_lastChar        ;BIOS variable of corese in ram
    mov     ax,BIOS_VARIABLES_SEGMENT
    mov     es,ax

key_repeat:
    

    
    mov     [ES:BX],al
    call    _KEY_PRESSED
    mov     dl,[ES:BX]
    mov     [ES:BX],al
    cmp     al,0
    jz      key_repeat
    cmp     al,0xF0
    jz      key_repeat  ;func key so ok to repeat
    cmp     dl,al
    jz      key_repeat
    

    mov     ah,0


    ret
Keyb_INIT:
    push ax
    push dx
    mov al,10000010b    
    mov dx,PPI_CTRL
    out dx,al     ;set port B to input port C high and low to output and port A to output
    call keyb_CAPS_OFF_
    pop dx
    pop ax
    ret
Keyb_CAPS_on:
keyb_CAPS_ON_:
    push ax
    push dx
    mov dx,PPI_CTRL
    mov al, 0x0C
    out dx,al
    pop dx
    pop ax
    ret
Keyb_CAPS_off:
keyb_CAPS_OFF_:
    push ax
    push dx
    mov dx,PPI_CTRL
    mov al, 0x0D
    out dx,al
    pop dx
    pop ax
    ret

keyb_SET_ROW_:
    push ax
    push dx
    xchg al,ah
    mov dx,PPI_PORT_C
    in  al,dx
    and ah,0x0F
    and al,0xF0
    or  al,ah
    out dx,al
    pop dx
    pop ax
    ret

keyb_GET_COL_:

    push dx
    mov dx,PPI_PORT_B
    in  al,dx
    mov ah,0
    pop dx

    ret
chk_toggle_caps:


    test    al,0x08
    jz     chk_toggle_caps_no
    test    ah,keyFlags_caps_last
    jz      chk_toggle
    or     ah,keyFlags_caps_last
    ret
chk_toggle:
    
    xor     ah,keyFlags_caps ;new state - fliped last state
    or     ah,keyFlags_caps_last
    test    ah,keyFlags_caps
    jz      chk_caps_off
    call    keyb_CAPS_ON_
    ret
chk_caps_off:
    call    keyb_CAPS_OFF_
    ret
chk_toggle_caps_no:
    and     ah,keyFlags_caps ;last time is now and is off , all zero except caps bit  
    ret

_update_kyb_flags:
update_kyb_flags:
    push    ax
    push    bx
    push    es
    mov     ax,BIOS_VARIABLES_SEGMENT
    mov     es,ax
    mov     bx,BIOS_VARIABLES_OFFSET
    add     bx,BVAR_kyebFlags
    mov     es,ax
    mov     al,0x06
    call    keyb_SET_ROW_
    call    keyb_GET_COL_
    
    mov     ah,[ES:BX] ;get flags
    mov     cl,keyFlags_caps
    or      cl,keyFlags_caps_last
    and     ah,cl  ;only caps stay
    not     al

    call    chk_toggle_caps
    
    test    al,0x01 ;shift
    jz      update_skip_shift
    or      ah,keyFlags_shift
update_skip_shift:

    test    al,0x02 ;ctrl
    jz      update_skip_ctrl
    or      ah,keyFlags_ctrl
update_skip_ctrl:

    test    al,0x04 ;alt
    jz      update_skip_alt
    or      ah,keyFlags_alt
update_skip_alt:

    test    al,0x10 ;CFn
    jz      update_skip_func
    or      ah,keyFlags_func
update_skip_func:

    mov     [ES:BX],ah ;update flags
    pop     es
    pop     bx
    pop     ax
    ret


Keyb_key_pressed:
_KEY_PRESSED:   ;stais for asm
    call    update_kyb_flags
grr_res:
    mov     cx,0xff
grr:
    inc  cl
    cmp  cl,9
    jz   no_key
    mov  al,cl
    call keyb_SET_ROW_
    call keyb_GET_COL_
    cmp  al,0xFF
    jz   grr
    ;int  06
    mov  bl,cl
    mov  ah,0
    mov  bh,0

    call _keyb_DECODE

    cmp     al,0x00
    jz   grr
    ;int  06
    ;jmp  grr_res
    mov  ah,0




    ;int 0x10
    ret

no_key:
    mov     ax,0
    ret

_keyb_DECODE:
;bx ; row
;ax ; col
    xchg    ax,bx
    push    ax
    mov     ax,BIOS_VARIABLES_SEGMENT
    mov     es,ax
    pop     ax
;need to convert inverted col to a binary number
    mov     bh,0

.loop:
    sar     bl,1
    jnc     .loopend
    inc     bh
    cmp     bh,8
    jz      __DECODE_FAIL
    jmp     .loop



.loopend:

    mov     bl,bh ;col in normal number

    mov     ah,0
    clc
    sal     al,1
    sal     al,1
    sal     al,1

    add     al,bl 
    mov     di,ax   ;final offset in di befor shift or caps
    mov     bx,BVAR_kyebFlags
    mov     ah,[ES:BX] ;keyboard flags in ah
    test    ah,keyFlags_shift
    jz      _decode_chek_caps
    add     di,11*8     ;offset for shift table
    jmp     _decode_skip_CAPS
_decode_chek_caps:
    mov     ah,[ES:BX] ;keyboard flags in ah
    test    ah,keyFlags_caps
    jz      _decode_skip_CAPS
    add     di,11*8*2     ;offset for caps table
_decode_skip_CAPS:
    
    lea     bx,KEY_DECODE
    mov     al,[CS:BX+di]
    mov     ah,0

    ret
__DECODE_FAIL:
    mov     ax,0x00

    ret

KEY_DECODE:
    db '0', '1', '2', '3', '4', '5', '6', '7'  ;ROW 0
    db '8', '9', '-', '=', '\', '[', ']', ';'  ;ROW 1 
    db 0x27,'`', ',', '.', '/',  0 , 'a', 'b'  ;ROW 2 ;0x27 = '
    db 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'  ;ROW 3
    db 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r'  ;ROW 4
    db 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'  ;ROW 5
    db  0,   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0   ;ROW 6
    db  0,   0 , 27 ,  11,  0 ,  8 ,  0 , 10   ;ROW 7
    db ' ',0xF0,0xF0, 127,0xF0,0xF0,0xF0,0xF0  ;ROW 8
    db '*', '+', '/', '0', '1', '2', '3', '4'  ;ROW 9
    db '5', '6', '7', '8', '9', '-', ',', '.'  ;ROW 10
    
    ;Shift
    db ')', '!', '@', '#', '$', '%', '^', '&'  ;ROW 0
    db '*', '(', '_', '+', '|', '{', '}', ':'  ;ROW 1 
    db '"','~', '<', '>', '?',  0 , 'A', 'B'  ;ROW 2 ;0x27 = '
    db 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'  ;ROW 3
    db 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'  ;ROW 4
    db 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'  ;ROW 5
    db  0,   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0   ;ROW 6
    db  0,   0 ,  1 ,  11,  0 ,  8 ,  0 , 10   ;ROW 7
    db ' ',0xF0,0xF0,  2,0xF0,0xF0,0xF0,0xF0  ;ROW 8
    db '*', '+', '/', '0', '1', '2', '3', '4'  ;ROW 9
    db '5', '6', '7', '8', '9', '-', ',', '.'  ;ROW 10

    ;CAPS
    db '0', '1', '2', '3', '4', '5', '6', '7'  ;ROW 0
    db '8', '9', '-', '=', '\', '[', ']', ';'  ;ROW 1 
    db 0x27,'`', ',', '.', '/',  0 , 'A', 'B'  ;ROW 2 ;0x27 = '
    db 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'  ;ROW 3
    db 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'  ;ROW 4
    db 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'  ;ROW 5
    db  0,   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0   ;ROW 6
    db  0,   0 , 27 ,  11,  0 ,  8 ,  0 , 10   ;ROW 7
    db ' ',0xF0,0xF0, 127,0xF0,0xF0,0xF0,0xF0  ;ROW 8
    db '*', '+', '/', '0', '1', '2', '3', '4'  ;ROW 9   ;never used because no key pad
    db '5', '6', '7', '8', '9', '-', ',', '.'  ;ROW 10