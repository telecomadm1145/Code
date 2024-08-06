#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

int main()
{
	reset_sfrs();
	while (1)
	{
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