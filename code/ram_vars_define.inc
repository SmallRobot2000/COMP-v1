RAM_VARS_BASE       equ     0
RAM_VARS_SEGMENT    equ     7000h
GAME_SS_BUF         equ     RAM_VARS_BASE+0   ;offset in DS
GAME_ADD_W          equ     RAM_VARS_BASE+1   ;2 bytes
GAME_ADD_R          equ     RAM_VARS_BASE+3   ;2 bytes
TMP_VAR             equ     RAM_VARS_BASE+5   ;1 byte
CHK_SUM             equ     RAM_VARS_BASE+6   ;1 byte

SPI_SS_BUF          equ     GAME_SS_BUF
DATA_BUFFER         equ     RAM_VARS_BASE+7     ;4k buffer

SPI_BLOCK_SIZE      equ     1000h