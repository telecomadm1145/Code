#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

int main()
{
	reset_sfrs();
	ScreenSelect = 4;
	while (1)
	{
		memcpy_nn((void __near*)0xf800,(void __near*)0,0x800);
		delay(300);
		memzero_n((void __near*)0xf800,0x800);
		delay(300);
	}
}