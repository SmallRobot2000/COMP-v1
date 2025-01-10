; Print a null-terminated string
print_string:
    mov dl,XM_DATA
    mov ah, 0x0F      ; BIOS teletype output function
.print_char:
    lodsb             ; Load byte at [SI] into AL and increment SI
    cmp al, 0         ; Check for null terminator
    je .done          ; If null, exit the function
    call    xo_write_reg
    jmp .print_char   ; Repeat for the next character
.done:
    ret
