#include "classwiz_bsp.h"
#include "libc.h"
#include "menu.h"

int main()
{
	memzero_n((void __near *)0x9000, ((ushort)0xef00 - (ushort)0x9000));
	reset_sfrs();
	delay(1600);
	FCON = 0x81;
	reset_screen_sfrs();
	while (1)
	{
		auto v = show_menu(test_menu, 13);
		memzero_n((void __near *)GetScreenBuffer(), 0x600 * 2);
		Color = 3;
		line_print_f("You clicked:",0,0);
		char buf[8];
		itoa_n(v+1,(char __near*)buf);
		line_print_f(buf,0,16);
		render_copy();
		wait_kiko();
	}
}