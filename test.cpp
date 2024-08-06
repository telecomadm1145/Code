#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

int main()
{
	reset_sfrs();
	while (1)
	{
		memzero_n(GetScreenBuffer(), 0x600 * 2);
		line_print_n((const char __near *)"888888888888888", 0, 1);
		line_print_n((const char __near *)"8.888888889\x90\x91\xa1\xa5", 30, 48);
		// line_print_n((const char __near *)"\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc", 0, 32);
		// line_print_n((const char __near *)"For Free", 0, 48);
		render_copy();
		ScreenSelect = 0;
		val(0xF803) = 1;
		val(0xF804) = 1;
		val(0xF812) = 1;
		ScreenSelect = 4;
		val(0xF803) = 1;
		val(0xF804) = 1;
		val(0xF812) = 1;
		auto kv = wait_kiko();
		DebugOutputString("Got key!\n");
		char buf[4];
		itoa_n(kv.ki, (char __near *)buf);
		DebugOutputString(buf);
		DebugOutputString(",");
		itoa_n(kv.ko, (char __near *)buf);
		DebugOutputString(buf);
		DebugOutputString("\n");
	}
}