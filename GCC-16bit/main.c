#include<stdint.h>
#include "string.h"
#include "system.h"

uint8_t arr[0x7FFF];

int main()
{	


	//char c = Keyb_get_key();
	Keyb_INIT();
	Keyb_CAPS_on();
	//char str[] = "Hello World!!!";
	//int i = strlen(str);
	my_printf("Mozda hello %d",33);
	
	while(1)
	{
		putchar(Keyb_get_key());
		
	}
	return 0;
}
