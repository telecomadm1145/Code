#include "classwiz_bsp.h"

extern "C"
{
	static void a();
}
#pragma INTERRUPT a 8
#pragma INTERRUPT a 10
#pragma INTERRUPT a 12
#pragma INTERRUPT a 14
#pragma INTERRUPT a 16
#pragma INTERRUPT a 18
#pragma INTERRUPT a 20
#pragma INTERRUPT a 22
#pragma INTERRUPT a 24
#pragma INTERRUPT a 26
#pragma INTERRUPT a 28
#pragma INTERRUPT a 30
#pragma INTERRUPT a 32
#pragma INTERRUPT a 34
#pragma INTERRUPT a 36
#pragma INTERRUPT a 38
#pragma INTERRUPT a 40
#pragma INTERRUPT a 42
#pragma INTERRUPT a 44
#pragma INTERRUPT a 46
#pragma INTERRUPT a 48
#pragma INTERRUPT a 50
extern "C"
{
	static void a() {}
}
void reset_timer()
{
	Timer0Counter = 0;
	Timer0Interval = 0x79e;
	Timer0Control = 0x10f;
	InterruptPending_W0 = 0;
	__EI();
}

void delay(ushort after_ticks)
{
	if ((FCON & 2) != 0)
		FCON &= 0xfd;
	__DI();
	Timer0Interval = after_ticks;
	Timer0Counter = 0;
	Timer0Control = 0x0101;
	InterruptPending_W0 = 0;
	StopAcceptor = 0x50;
	StopAcceptor = 0xa0;
	StopControl = 2;
	__asm("nop");
	__asm("nop");
	__EI();
}
void reset_sfrs()
{
	FCON = 0x31;
	BlockControl = 0xf7;
	volatile int i = 0x32;
	while (i--)
	{
	}
	InterruptMask_W0 = 0x22;
	InterruptMask_W1 = 0;
	ScreenPower = 7;
	delay(200);
	ScreenRange = 4;
	ScreenBrightness = 3; // 7
	ScreenInterval = 6;
	ScreenUnk1 = 0x17;
	ScreenUnk2 = 8;
	ScreenOffset = 0;
	ScreenMode = 0x55;
	ScreenContrast = 0x12;
}