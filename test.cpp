#include "classwiz_bsp.h"
#include "libc.h"
#include "menu.h"
#include "textbox.h"
#include "UnicodeRender.h"

void set_pixel(byte x, byte y)
{
	*(byte __near *)(0xf800 + (ushort)y * 0x20 + (x / 8)) |= 0x80 >> (x & 7);
}

extern "C" float __fmulu8lw(float x, float y);

int main()
{
	memzero_n((void __near *)0x9000, ((ushort)0xef00 - (ushort)0x9000));
	reset_sfrs();
	delay(1600);
	FCON = 0x81;
	reset_screen_sfrs();
	memzero_n((void __near *)GetScreenBuffer(), 0x600 * 2);
	render_copy();
	for (int i = 0; i < 192; i++)
	{
		set_pixel(i, cosf((i - 96) / 10.0f) * 12.0f + 24.0f);
	}
	__asm("brk");
}
