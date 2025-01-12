#include<stdint.h>
#include <stdarg.h>
#define XM_SYS_CTRL     0x00
#define XM_INT_CTRL     0x01
#define XM_TIMER        0x02
#define XM_RD_XADDR     0x03
#define XM_WR_XADDR     0x04
#define XM_XDATA        0x05
#define XM_RD_INCR      0x06
#define XM_RD_ADDR      0x07
#define XM_WR_INCR      0x08
#define XM_WR_ADDR      0x09
#define XM_DATA         0x0A
#define XM_DATA_2       0x0B
#define XM_PIXEL_X      0x0C
#define XM_PIXEL_Y      0x0D
#define XM_UART         0x0E
#define XM_FEATURE      0x0F
//Text colors
#define BLACK           0x00
#define BLUE            0x01
#define GREEN           0x02
#define CYAN            0x03
#define RED             0x04
#define MAGENTA         0x05
#define BROWN           0x06
#define LIGHT_GRAY      0x07
#define DARK_GRAY       0x08
#define LIGHT_BLUE      0x09
#define LIGHT_GREEN     0x0A
#define LIGHT_CYAN      0x0B
#define LIGHT_RED       0x0C
#define LIGHT_MAGENT    0x0D
#define YELLOW          0x0E
#define WHITE           0x0F

#define TEXT_COLS       0x50
#define TEXT_ROWS       0x1E
#define TEXT_BASE_VRAM  0x4000

#define XM_TILE_COL_MAP 0x8000

extern void     XoWriteReg(int reg, int data);  
extern uint16_t XoReadReg(int reg);  

//global Keyb_INIT, Keyb_get_key, Keyb_key_pressed, Keyb_CAPS_on ,Keyb_CAPS_off
extern void     Keyb_INIT();
extern uint16_t Keyb_get_key();
extern uint16_t Keyb_key_pressed();
extern void     Keyb_CAPS_on();
extern void     Keyb_CAPS_off();

extern char     _KEY_PRESSED();
extern void     _asm_reset();

uint16_t _FORGROUND_COLOR;
uint16_t _BCKGROUND_COLOR;

uint8_t  _text_x;
uint8_t  _text_y;

uint16_t palettes[8][16];

#define TEXT_PALETTE_INDEX 0x00
const uint16_t text_palette[16] = {
    0xF000,  // Black
    0xF00A,  // Blue
    0xF0A0,  // Green
    0xF0AA,  // Cyan
    0xFA00,  // Red
    0xFA0A,  // Magenta
    0xFA50,  // Brown
    0xFAAA,  // Light Gray
    0xF555,  // Dark Gray
    0xF55F,  // Light Blue
    0xF5F5,  // Light Green
    0xF5FF,  // Light Cyan
    0xFF55,  // Light Red
    0xFF5F,  // Light Magenta
    0xFFF5,  // Yellow
    0xFFFF   // White
};

void setTextColor(uint8_t FR, uint8_t BG)
{
    _FORGROUND_COLOR = FR;
    _BCKGROUND_COLOR = BG;
}


void setCursorPos(uint8_t x, uint8_t y)
{
    XoWriteReg(XM_WR_ADDR, (y*TEXT_COLS+x)+TEXT_BASE_VRAM);
    _text_x = x;
    _text_y = y;
}

void putchar(char c)
{
    if(c == 0)
    {
        return;
    }
    if(c == '\n')
    {
        setCursorPos(0, _text_y+1);
    }else if(c == 1)
    {
        setCursorPos(0,0);
    }
    else if(c == '\b'){
        if(_text_x == 0)
        {
            if(_text_y == 0)
            {
                setCursorPos(0,0);
                putchar(' ');
                setCursorPos(0,0);
            }else{
                setCursorPos(TEXT_COLS-1,_text_y-1);
                putchar(' ');
                setCursorPos(TEXT_COLS-1,_text_y-1);
            }

        }else{
            setCursorPos(_text_x-1,_text_y);
            putchar(' ');
            setCursorPos(_text_x-1,_text_y);
        }
        setCursorPos(_text_x,_text_y);
    
    }else
    {
        setCursorPos(_text_x,_text_y);
        XoWriteReg(XM_DATA, (((_FORGROUND_COLOR)|(_BCKGROUND_COLOR<<4))<<8)|(c&0x00FF));
        _text_x++;
    }
    if(_text_x >= TEXT_COLS)
    {
        _text_x = 0;
        _text_y++;
    }
      
}

// Helper function to print an integer
void print_int(int num) {
    if (num == 0) {
        putchar('0');
        return;
    }

    if (num < 0) {
        putchar('-');
        num = -num;
    }

    char buffer[10];
    int i = 0;

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        putchar(buffer[--i]);
    }
}

// Helper function to print a string
void print_str(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}

// Custom printf function using only putchar
void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    for (const char *p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++; // Skip the '%' character

            switch (*p) {
                case 'd': {
                    int i = va_arg(args, int);
                    print_int(i);
                    break;
                }
                case 's': {
                    char *s = va_arg(args, char *);
                    print_str(s);
                    break;
                }
                default:
                    putchar('%');
                    putchar(*p);
                    break;
            }
        } else {
            putchar(*p);
        }
    }

    va_end(args);
}

void printAT(uint8_t x, uint8_t y, char *str)
{
    setCursorPos(x,y);
    print_str(str);
}

void CLS(uint8_t BG)
{
    setCursorPos(0,0);
    setTextColor(_FORGROUND_COLOR, BG);
    for(int i = 0;i < TEXT_COLS*TEXT_ROWS; i ++)
    {
        XoWriteReg(XM_DATA, 0x0000+(BG<<12));
    }
    setCursorPos(0,0);
}

void border(uint16_t BC)
{
    XoWriteReg(XM_WR_XADDR,0x0000);
    XoWriteReg(XM_XDATA,0xFFFF);
}

void sleep(uint16_t milis) //good up to araund 6500
{
    uint16_t end = milis*10+XoReadReg(XM_TIMER); //because 0.1ms interval
    while(XoReadReg(XM_TIMER) != end);
}

char getchar()
{
    static char oldC = 0xFF;
    char c = _KEY_PRESSED();
    while(c == 0 || oldC == c)
    {
        if(c == oldC)
        {
            sleep(90);
        }
        c = _KEY_PRESSED();
        oldC = c;
        if(oldC == c && c != 0)
        {
            break;
        }
        
    }


    return c;
}



//decompressor



void decompressImage_and_load16C(const uint8_t *data, const uint16_t *pallete, unsigned long size, uint16_t VRAM_add, int palette) {
    XoWriteReg(XM_WR_ADDR,VRAM_add);
    uint16_t dataCnt = 0;
    dataCnt = 0;
    //my_printf("%d",dataCnt); //Debug
    uint16_t  dataBuf[4];
    for (unsigned long i = 0; i < size; i += 2) {
        unsigned char value = data[i];
        unsigned char count = data[i + 1];
        
        for (unsigned char j = 0; j < count; j++) {
            
            //printf("%c", value);
            dataBuf[dataCnt] = value;
            dataCnt++;
            if(dataCnt == 2)
            {
                dataCnt = 0;
                XoWriteReg(XM_DATA,(dataBuf[0]&0x00FF)<<8|(dataBuf[1]&0x00FF));
                //XoWriteReg(XM_DATA,dataBuf[0]<<4*3|dataBuf[1]<<4*2|dataBuf[2]<<4*1|dataBuf[3]);
            }
        }
    }

    for(int i = 0; i<16 ; i++)
    {
        palettes[palette][i] = pallete[i];
    }
}


void setGFX(uint16_t VRAM_offset, int palette)
{
	XoWriteReg(XM_WR_XADDR,0x12);
    XoWriteReg(XM_XDATA,VRAM_offset);
	XoWriteReg(XM_WR_XADDR,0x10);
    XoWriteReg(XM_XDATA,0x0055); //bitmap 4bpp 2x scaling

	XoWriteReg(XM_WR_XADDR,XM_TILE_COL_MAP); 
    for(int i = 0; i<16 ; i++)
    {
        XoWriteReg(XM_XDATA,palettes[palette][i]); 
    }
}
void setTEXT(uint16_t VRAM_offset, int palette)
{
	XoWriteReg(XM_WR_XADDR,0x12);
    XoWriteReg(XM_XDATA,VRAM_offset);
	XoWriteReg(XM_WR_XADDR,0x10);
    XoWriteReg(XM_XDATA,0x0000); //text mode no scaling

	XoWriteReg(XM_WR_XADDR,XM_TILE_COL_MAP); 
    for(int i = 0; i<16 ; i++)
    {
        XoWriteReg(XM_XDATA,palettes[palette][i]); 
    }
}