#include "classwiz_bsp.h"
#include "libc.h"
#include "menu.h"
#include "UnicodeRender.h"

#include "sd/sd.h"
#include "sd/fat.h"

int main()
{
	memzero_n((void __near *)0x9000, ((ushort)0xef00 - (ushort)0x9000));
	reset_sfrs();
	delay(1600);
	FCON = 0x81;
	reset_screen_sfrs();
	memzero_n((void __near *)GetScreenBuffer(), 0x600 * 2);
	render_copy();

	sd_init();

	unsigned long cap = 0;
	sd_get_info(&cap);
	DebugOutputString("Capacity: ");
	DebugOutputInt(cap);
	if (init_fat32())
		DebugOutputString("Fat32 mounted!\n");

	list_directory(fat32_params.root_cluster);

	DrawTextWithWrapping((const wchar_t __near*)L"玩原神玩的",0,0);
	__asm("brk");
}