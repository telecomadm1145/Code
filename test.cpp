#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

int main()
{
	reset_sfrs();
	while (1)
	{
		line_print_n((const char __near *)"Play", 0, 1);
		line_print_n((const char __near *)"Genshin", 0, 16);
		line_print_n((const char __near *)"Impact", 0, 32);
		line_print_n((const char __near *)"For Free", 0, 48);
		// render_copy();
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