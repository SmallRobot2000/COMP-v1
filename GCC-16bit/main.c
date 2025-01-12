#include<stdint.h>
#include "string.h"
#include "system.h"
#include "../test data and utils/Utils/amiga.h"
#include "../test data and utils/Utils/self2.h"
//#include"../code/C/new/self.h"


#define IMAGE0_BASE 0x5000
#define IMAGE1_BASE 0xA000
void imageDemo()
{
	char c;
	while(1)
	{
		setGFX(IMAGE0_BASE,1);
		c = Keyb_get_key();
		if(c == 'q')
		{
			break;
		}
		setGFX(IMAGE1_BASE,2);
		c = Keyb_get_key();
		if(c == 'q')
		{
			break;
		}
	}
	

	setTEXT(TEXT_BASE_VRAM,TEXT_PALETTE_INDEX);

	CLS(BLUE);
	setTextColor(YELLOW,BLUE);
	setCursorPos(0,0);
	my_printf("COMP v1 alfa\n");
	
}
void textDemo()
{
	CLS(BLUE);
	setTextColor(YELLOW,BLUE);
	int col = 0;
	for(int i = 0; i < TEXT_ROWS; i ++)
	{
		setTextColor(col,col);
		setCursorPos(0,i);
		my_printf("                                                                                ");
		setTextColor(WHITE,col);
		setCursorPos(0,i);
		my_printf("%d",i+1);
		col ++;
		if(col == 16){
			col = 0;
		}
	}
	setTextColor(WHITE,BLACK);
		setCursorPos(0,0);
		my_printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890");

	sleep(1000);
	for(int i = 6; i < 24; i++)
	{
		setCursorPos(8,i);
		my_printf("                                                                ");
	}
	
	setTextColor(YELLOW,BLACK);
	setCursorPos(9,7);
	my_printf("CPU:           Intel 8088 @ 5Mhz");
	setCursorPos(9,10);
	my_printf("MEMORY:        512K RAM + 512K RAM/32K ROM");
	setCursorPos(9,13);
	my_printf("AUDIO/VIDEO:   XOSERA - 640x480 @ 60Hz - 4ch @ 48Khz (stereo)");
	setCursorPos(9,16);
	my_printf("SERIAL:        USB FTDI 115200 bps/8 bit no parity 1 stop bit");
	setCursorPos(9,19);
	my_printf("KEYBOARD:      MSX-like kyboard no rollover");
	setCursorPos(9,22);
	my_printf("CREATED BY:    Lovro Lopatic, TSRB ZG, 2024/2025");

	char c = Keyb_get_key();
	CLS(BLUE);
	setTextColor(YELLOW,BLUE);
	setCursorPos(0,0);
	my_printf("COMP v1 alfa\n");

	
	
}


int main()
{	
	Keyb_INIT();
	CLS(BLUE);
	setTextColor(YELLOW,BLUE);


    for(int i = 0; i<16 ; i++)
    {
        palettes[0][i] = text_palette[i];
    }
	setTEXT(TEXT_BASE_VRAM,TEXT_PALETTE_INDEX);
	setCursorPos(0,0);
	my_printf("COMP v1 alfa\n");
	my_printf("Loading images\n");
	setCursorPos(0,2);
	my_printf("0/2\n");
	decompressImage_and_load16C(compressed_data, palette, compressed_size, IMAGE0_BASE, 1);
	setCursorPos(0,2);
	my_printf("1/2\n");
	decompressImage_and_load16C(compressed_data1, palette1, compressed_size1, IMAGE1_BASE, 2);
	setCursorPos(0,2);
	my_printf("2/2\n");

	setCursorPos(0,3);
	setTextColor(YELLOW,BLUE);
	char inStr[256];
	inStr[0] = 0;
	int inStrCnt = 0;
	while(1)
	{

		char c = Keyb_get_key();
		if(c == 2)
		{
			CLS(BLACK);
			//my_printf("Shoud reset");
			_asm_reset();
		}
		if(c == '\n')
		{	

			if(inStr[0] != 0)
			{
				inStr[inStrCnt] = 0;
				//my_printf("%d",strcmp(inStr,"image 0"));
				if(strcmp(inStr,"image demo") == 0)
				{
					imageDemo();
					
					inStr[0] = 0;
				}else if(strcmp(inStr,"text demo") == 0)
				{
					textDemo();
					inStr[0] = 0;
				}else{
					inStr[0] = 0;
				}
			}
			inStrCnt = 0;
		}else if(c != '\b'){
			inStr[inStrCnt] = c;
			inStrCnt++;
		}

		if(c == '\b' && inStrCnt > 0)
		{
			inStrCnt--;
		}
		putchar(c);
		
	}
	return 0;
}
