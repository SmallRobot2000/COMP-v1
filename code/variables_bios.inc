BIOS_VARIABLES_OFFSET equ 0x0000
BIOS_VARIABLES_SEGMENT equ 0x7000
BVAR_lastChar   equ 0x0000 ;1 byte
BVAR_kyebFlags  equ 0x0001 ;1 byte ;flags: bit   7   6   5   4   3   2   1   0
keyFlags_shift  equ 0x01           ;                        Alt  Fn crl caps shift
keyFlags_caps   equ 0x02
keyFlags_ctrl   equ 0x04
keyFlags_func   equ 0x08
keyFlags_alt    equ 0x10
keyFlags_caps_last    equ 0x20 ;for toggle
