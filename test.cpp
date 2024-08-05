#include "classwiz_bsp.h"
#include "libc.h"
extern "C" void render_copy();

int main()
{
	reset_sfrs();
	while (1)
	{
		render_copy();
		delay(100);
	}
}