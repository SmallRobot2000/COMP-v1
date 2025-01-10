#include<stdint.h>
#include <stdarg.h>
#define XM_SYS_CTRL      0x00
#define XM_INT_CTRL      0x01
#define XM_TIMER         0x02
#define XM_RD_XADDR      0x03
#define XM_WR_XADDR      0x04
#define XM_XDATA         0x05
#define XM_RD_INCR       0x06
#define XM_RD_ADDR       0x07
#define XM_WR_INCR       0x08
#define XM_WR_ADDR       0x09
#define XM_DATA          0x0A
#define XM_DATA_2        0x0B
#define XM_PIXEL_X       0x0C
#define XM_PIXEL_Y       0x0D
#define XM_UART          0x0E
#define XM_FEATURE       0x0F

extern void     XoWriteReg(int reg, int data);  
extern uint16_t XoReadReg(int reg);  

//global Keyb_INIT, Keyb_get_key, Keyb_key_pressed, Keyb_CAPS_on ,Keyb_CAPS_off
extern void     Keyb_INIT();
extern uint16_t     Keyb_get_key();
extern uint16_t     Keyb_key_pressed();
extern void     Keyb_CAPS_on();
extern void     Keyb_CAPS_off();


void putchar(char c)
{
    if(c == '\n')
    {
        XoWriteReg(XM_DATA, 0xF000+'N');
        XoWriteReg(XM_DATA, 0xF000+'L');
    }
    XoWriteReg(XM_DATA, 0xF000+c);  
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

