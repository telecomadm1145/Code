#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

int main()
{
	char test[30] = "Count:";
	reset_sfrs();
	Timer0Interval = 65535;
	ushort n = itoa_n(atoi_n<long>( (char __near *)"07210721"), (char __near *)(test + 6));
	*(test + n + 6) = '\n';
	*(test + n + 6 + 1) = 0;
	DebugOutputString(test);
	while (1)
		delay(1000);
}