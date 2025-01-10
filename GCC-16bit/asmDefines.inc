;Xosera 
XOSERA_IOBASE   equ 080h
XOSERA_NIBBLE_L equ XOSERA_IOBASE+0
XOSERA_NIBBLE_H equ XOSERA_IOBASE+1
;Xosera main registers

XM_SYS_CTRL     equ 00h
XM_INT_CTRL     equ 01h
XM_TIMER        equ 02h
XM_RD_XADDR     equ 03h
XM_WR_XADDR     equ 04h
XM_XDATA        equ 05h
XM_RD_INCR      equ 06h
XM_RD_ADDR      equ 07h
XM_WR_INCR      equ 08h
XM_WR_ADDR      equ 09h
XM_DATA         equ 0Ah
XM_DATA_2       equ 0Bh
XM_PIXEL_X      equ 0Ch
XM_PIXEL_Y      equ 0Dh
XM_UART         equ 0Eh
XM_FEATURE      equ 0Fh

;0x0 XM_SYS_CTRL (R/W+) - System Control
;0x1 XM_INT_CTRL (R/W+) - Interrupt Control
;0x2 XM_TIMER (R/W) - Timer Functions
;0x3 XM_RD_XADDR (R/W+) - XR Read Address
;0x4 XM_WR_XADDR (R/W) - XR Write Address
;0x5 XM_XDATA (R+/W+) - XR Read/Write Data
;0x6 XM_RD_INCR (R/W) - Increment for VRAM Read Address
;0x7 XM_RD_ADDR (R/W+) - VRAM Read Address
;0x8 XM_WR_INCR (R/W) - Increment for VRAM Write Address
;0x9 XM_WR_ADDR (R/W) - VRAM Write Address
;0xA XM_DATA (R+/W+) - VRAM Read/Write Data
;0xB XM_DATA_2 (R+/W+) - VRAM Read/Write Data (2nd)
;0xC XM_PIXEL_X (-/W+) - X coordinate for pixel addr/mask generation (also used to set PIXEL_BASE)
;0xD PIXEL_Y (-/W+) - Y coordinate for pixel addr/mask generation (also used to set PIXEL_WIDTH)
;0xE XM_UART (R+/W+)
;0xF XM_FEATURE (R/-) - Xosera feature bits

%include"../code/variables_bios.inc"
%include"../code/keyboard.inc"