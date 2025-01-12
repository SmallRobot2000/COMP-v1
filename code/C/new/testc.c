#include <stddef.h>  // For size_t

#include "test_sample.h"
//#include "amigaOutput.h"
#include "self.h"
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

int chrXpos;
int chrYpos;
extern void __cdecl XoWriteReg(int reg, int data);  //__cdecl means that arguments are on stack right to left first on stack is first parametar
extern uint16_t  __cdecl XoReadReg(int reg);             //__cdecl arguments are returned in AX
extern char __cdecl foo(); 
extern void testkeyb();
extern void keyb_CAPS_OFF();
extern void keyb_CAPS_ON();
extern void keyb_init();
extern void keyb_SET_ROW(int row);
extern int  keyb_GET_COL();

extern void print_hexbyte(int num);
extern char __cdecl KEY_PRESSED();
extern char __cdecl GET_KEY();
extern void __cdecl update_kyb_flags();
size_t strlen(char *str) {
    size_t length = 0;

    // Loop through the string until the null terminator is found
    while (str[length] != '\0') {
        length++;  // Increment the length for each character
    }

    return length;  // Return the length of the string
}
void printC(char c)
{


        if(c == '\n')
        {
            chrXpos = 0;
            chrYpos++;
            XoWriteReg(XM_WR_ADDR,0x4000+chrYpos*80);
        }else{
            XoWriteReg(XM_DATA,0x4700+c);
        }
        
        chrXpos++;
        if(chrXpos >= 80)
        {
            chrXpos = 0;
            chrYpos++;
            XoWriteReg(XM_WR_ADDR,0x4000+chrYpos*80);
        }
        
    
}
void printS(char *str)
{
    int i = 0;

    for(i = 0;i < strlen(str);i++)
    {
        if(str[i] == '\n')
        {
            chrXpos = 0;
            chrYpos++;
            XoWriteReg(XM_WR_ADDR,0x4000+chrYpos*80);
        }else{
            XoWriteReg(XM_DATA,0x4700+str[i]);
        }
        
        chrXpos++;
        if(chrXpos >= 80)
        {
            chrXpos = 0;
            chrYpos++;
            XoWriteReg(XM_WR_ADDR,0x4000+chrYpos*80);
        }
        
    }
    i = 0;
}

void waitVblank()
{
    int tmp;
    while((tmp&0x0800)==0)
    {
        tmp = XoReadReg(XM_SYS_CTRL);
    }
    while((tmp&0x0400)==0)
    {
        tmp = XoReadReg(XM_SYS_CTRL);
    }
}

int main() {
    int notSmallChehe = 79;
    int arr[3];
    long tmp = 2;
    int col;
    char charr[2];
    char c;
    char c_old;
    
    chrYpos = 0;
    chrXpos = 0;

    charr[1] = 0;
    
    //if(tmp = 2)
    //{
    //    printS("Opebn watcom is not goof");
    //}
    //testkeyb();
    //int i;  // Local variable
    //XoWriteReg(0x0A,0x4700);
    while((tmp&0x0800)==0)
    {
        tmp = XoReadReg(XM_SYS_CTRL);
    }
    while((tmp&0x0400)==0)
    {
        tmp = XoReadReg(XM_SYS_CTRL);
    }


    XoWriteReg(XM_WR_ADDR,0x4000);
    
    printS("No\nVI\nRe\nDo\nVI\n!!!!!"); //80
    printS("Hello World form C!!!!");
    XoWriteReg(0x0A,0x4700+arr[0]);
    printS("Ima problema sa localnim vari\nablmama idk why");
    //foo('G');  // Call the external assembly function
    if(notSmallChehe != 79)
    {
        printS("How?????");

    }else{
        printS("Cudni su ti stringovi");
    }
    


    keyb_init();    
    printS("Zej test\n");
    

    printS("\n\nLoading image...\n");
    arr[0] = 'y';
    XoWriteReg(XM_WR_XADDR,0x02);
    XoWriteReg(XM_XDATA,0x01); //Enable audio
    XoWriteReg(XM_WR_XADDR,0x20);
    XoWriteReg(XM_XDATA,0x8080); //Volume ch0 to 100%
    XoWriteReg(XM_XDATA,1256); //araund 1 khz
    //XoWriteReg(XM_XDATA,4096); //8192 samples (4096*2)
    XoWriteReg(XM_XDATA,0x0); //start add is 0
    
    XoWriteReg(XM_WR_ADDR,0x0000);

    for(tmp = 0;tmp<8192;tmp++)
    {
        XoWriteReg(XM_DATA,test_sample[tmp]); //start add is 0
    }
    

    
    



    
    XoWriteReg(XM_WR_ADDR,0x8000);
    for(tmp = 0;tmp<sizeof(test_image);tmp+=2)
    {
        XoWriteReg(XM_DATA,test_image[tmp]<<8|test_image[tmp+1]); //start add is 0
    }

    XoWriteReg(XM_WR_XADDR,0x8000);
    for(tmp = 0;tmp<256/2;tmp++)
    {
        XoWriteReg(XM_XDATA,test_palette[tmp]); //start add is 0
    }
    c_old = 255;
    XoWriteReg(XM_WR_XADDR,0x12);
    XoWriteReg(XM_XDATA,0x8000);
    XoWriteReg(XM_WR_XADDR,0x10);
    XoWriteReg(XM_XDATA,0x0055); //bitmap 4bpp 2x scaling

    while(1)
    {   
        update_kyb_flags();
        printC(GET_KEY());
        //XoWriteReg(XM_WR_XADDR,0x21);
        //XoWriteReg(XM_XDATA,612|0x8000); //araund 1 khz //with force reset
    }

    //while(1);
    return 0;
}
