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
	ExternalInterruptControl = 0;
}

bool scan_key(kiko __near *er8)
{
	byte r1 = 0x01;
	byte r3 = 0x01;
	byte r2;

	while (r3 <= 0x07)
	{
		KeyboardOut = r1;
		r2 = KeyboardIn;

		if (r2 == 0xFF)
		{
			r1 <<= 1;
			r3++;
		}
		else
		{
			er8->ko = r1;
			er8->ki = ~r2;
			return 1;
		}
	}

	return r3 <= 0x07;
}
byte key_debounce(kiko __near *er8)
{
	byte r5 = 0x00;
	byte r6 = 0x01;

	while (r6 <= 5)
	{
		delay(10);

		KeyboardOut = er8->ko;
		byte r4 = ~KeyboardIn;

		KeyboardOut = 0;

		if ((r4 & er8->ki) != 0)
		{
			r5++;
			er8->ki &= r4;
		}

		r6++;
	}

	return r5 == 0x05;
}

kiko wait_kiko()
{
	kiko kv;
redo:
	KeyboardOut = 0xff;
	KeyboardInMask = 0xff;
	while (1)
	{
		delay(4000);
		if (InterruptPending0 & 2)
		{
			if (scan_key((kiko __near *)&kv) && key_debounce((kiko __near *)&kv) && kv.ki != 0)
				return kv;
			goto redo;
		}
	}
}
const byte normal_font[3641] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x78, 0xDE, 0xFD, 0x86, 0xDF, 0x06, 0x0F, 0x07, 0x1E, 0x03, 0x3C, 0xC1, 0x7E, 0xF3, 0xEF, 0xBF, 0xC7, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x33, 0xF8, 0x37, 0x73, 0x70, 0xE3, 0x60, 0xC0, 0xE1, 0x80, 0xC3, 0x00, 0xC7, 0xF8, 0x8F, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1E, 0x00, 0x1E, 0x33, 0x3F, 0x33, 0x3F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x1F, 0xE3, 0x1F, 0xC3, 0x0F, 0xC3, 0x0F, 0x83, 0x07, 0x83, 0x07, 0x03, 0x83, 0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xC0, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x3F, 0x80, 0x7F, 0x80, 0x6D, 0x01, 0x6C, 0x06, 0x7F, 0x98, 0x3F, 0x86, 0x0D, 0x81, 0x6D, 0x80, 0x7F, 0x00, 0x3F, 0x00, 0x0C, 0x00, 0x00, 0xC0, 0x03, 0xE0, 0x07, 0x60, 0x06, 0xE1, 0x66, 0xC1, 0xA7, 0x80, 0x23, 0xD8, 0x27, 0xD8, 0x2E, 0xF8, 0xAC, 0x70, 0x6C, 0xFC, 0x0F, 0xDC, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0x60, 0x7E, 0x60, 0x7E, 0xE0, 0x18, 0xC0, 0x18, 0xCC, 0x18, 0xFC, 0x18, 0x78, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x86, 0x0F, 0x87, 0x0F, 0x07, 0x06, 0x06, 0x06, 0x86, 0x07, 0x86, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xC0, 0xC0, 0xF8, 0xE0, 0x0C, 0x60, 0xFC, 0x60, 0xF8, 0x60, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x60, 0x03, 0x60, 0x03, 0x60, 0x03, 0x60, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x40, 0x36, 0xE1, 0x36, 0x73, 0xFF, 0x3F, 0xFF, 0x1E, 0x6C, 0x1E, 0x6C, 0x3F, 0xFE, 0x73, 0xFE, 0xE1, 0xD8, 0x40, 0xD8, 0x00, 0xD8, 0x00, 0x00, 0x60, 0x1C, 0xE0, 0xB6, 0xC0, 0xF6, 0xC0, 0xB7, 0x80, 0x1D, 0x8F, 0x03, 0x0F, 0x07, 0xE0, 0x06, 0xB0, 0x8F, 0xB0, 0xCD, 0xB0, 0x9D, 0xE0, 0x18, 0x00, 0x00, 0x30, 0xC0, 0x30, 0xC0, 0x30, 0xC0, 0x30, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x00, 0x0C, 0x03, 0x0E, 0x07, 0x07, 0x0E, 0x03, 0x0C, 0x03, 0x1C, 0x01, 0x18, 0x01, 0x18, 0x03, 0x1C, 0x03, 0x0C, 0x07, 0x0E, 0x0E, 0x07, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x80, 0xE3, 0x81, 0xE3, 0x81, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x30, 0x38, 0x30, 0x38, 0x30, 0x38, 0x30, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xF0, 0x3F, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1C, 0x00, 0x18, 0x00, 0x38, 0x00, 0x30, 0x00, 0x30, 0x00, 0x70, 0x00, 0x60, 0x80, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x03, 0x3F, 0x87, 0x7F, 0x8F, 0x61, 0x8D, 0x61, 0x81, 0x61, 0x81, 0x61, 0x81, 0x61, 0x81, 0x61, 0x81, 0x61, 0x81, 0x61, 0x81, 0x7F, 0x01, 0x3F, 0x00, 0x00, 0xF0, 0x83, 0xF9, 0x87, 0x19, 0x86, 0x18, 0x80, 0x18, 0x80, 0x38, 0x80, 0xF0, 0x80, 0xE0, 0x81, 0x80, 0x83, 0x01, 0x87, 0xF9, 0x87, 0xF8, 0x87, 0x00, 0x00, 0x07, 0xFC, 0x0F, 0xFE, 0x1F, 0x86, 0x1B, 0x06, 0x3B, 0x06, 0x33, 0x7C, 0x73, 0x7E, 0x63, 0x06, 0x7F, 0x06, 0x7F, 0x86, 0x03, 0xFE, 0x03, 0xFC, 0x00, 0x00, 0xE3, 0x1F, 0xE7, 0x1F, 0x06, 0x18, 0x06, 0x18, 0xC6, 0x1F, 0xE7, 0x1F, 0x67, 0x00, 0x66, 0x00, 0x66, 0x80, 0x66, 0x98, 0xE7, 0x1F, 0xC3, 0x0F, 0x00, 0x00, 0xFE, 0xF1, 0xFE, 0xF9, 0x06, 0x18, 0x0E, 0x00, 0x0C, 0x00, 0x1C, 0xF0, 0x18, 0xF8, 0x38, 0x18, 0x30, 0x18, 0x70, 0x18, 0x60, 0xF8, 0x60, 0xF0, 0x00, 0x00, 0x0F, 0x3F, 0x9F, 0x7F, 0x98, 0x61, 0x98, 0x61, 0x98, 0x61, 0x1F, 0x3F, 0x8F, 0x7F, 0x80, 0x61, 0x80, 0x61, 0x98, 0x61, 0x9F, 0x7F, 0x0F, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0xE0, 0x60, 0xE0, 0x60, 0xE0, 0x60, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x60, 0xE0, 0x60, 0xE0, 0x60, 0xE0, 0xE0, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x38, 0x0E, 0x38, 0x1C, 0x38, 0x38, 0x00, 0x30, 0x00, 0x38, 0x00, 0x1C, 0x38, 0x0E, 0x38, 0x07, 0x38, 0x03, 0x30, 0x00, 0x30, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0xF1, 0x3F, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xF0, 0x3F, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x78, 0x00, 0xFC, 0x00, 0xCC, 0x80, 0x0C, 0xC0, 0x18, 0xE0, 0x18, 0x70, 0x30, 0x30, 0x30, 0x70, 0x00, 0xE0, 0x00, 0xC0, 0x30, 0x80, 0x30, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x83, 0x7F, 0xC7, 0xE1, 0xC6, 0xCE, 0xCE, 0xDE, 0xCC, 0xDA, 0xCC, 0xDA, 0xDF, 0xDA, 0xDF, 0xDF, 0x98, 0xCD, 0x18, 0xE0, 0x98, 0x7F, 0x80, 0x3F, 0xF0, 0x87, 0xF8, 0x87, 0x19, 0xC6, 0x19, 0xC6, 0x19, 0xE6, 0xF1, 0x67, 0xF9, 0x67, 0x19, 0xF6, 0x19, 0xF6, 0x19, 0x36, 0xF8, 0x37, 0xF0, 0x37, 0x00, 0x00, 0x7E, 0x7C, 0x7F, 0xFE, 0x63, 0xC6, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x63, 0xC6, 0x7F, 0xFE, 0x7E, 0x7C, 0x00, 0x00, 0xE7, 0x1F, 0xE7, 0x1F, 0x06, 0x98, 0x06, 0x98, 0x06, 0x98, 0xC7, 0x9F, 0xC7, 0x9F, 0x06, 0x98, 0x06, 0x98, 0x06, 0x98, 0xE6, 0x1F, 0xE6, 0x1F, 0x00, 0x00, 0x7C, 0xF8, 0xFE, 0xF8, 0xC6, 0x01, 0x80, 0x01, 0x80, 0x01, 0x9E, 0xF1, 0x9E, 0xF1, 0x86, 0x01, 0x86, 0x01, 0xC6, 0x01, 0xFE, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x8F, 0x61, 0x8F, 0x61, 0x83, 0x61, 0x83, 0x61, 0x83, 0x61, 0x83, 0x7F, 0x83, 0x7F, 0x83, 0x61, 0x83, 0x61, 0x83, 0x61, 0x8F, 0x61, 0x8F, 0x61, 0x00, 0x00, 0x31, 0xC0, 0x31, 0xC0, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x71, 0xC0, 0xE1, 0xC3, 0xC0, 0x03, 0x60, 0x86, 0x60, 0x8E, 0x60, 0x9C, 0x60, 0xB8, 0x60, 0xF0, 0x60, 0xF8, 0x60, 0xD8, 0x60, 0x9C, 0x60, 0x8C, 0x60, 0x8E, 0x7F, 0x86, 0x7F, 0x86, 0x00, 0x00, 0x66, 0x18, 0x67, 0x18, 0x67, 0x18, 0xE7, 0x1C, 0xE7, 0x1C, 0xE6, 0x1C, 0xE6, 0x1F, 0xE6, 0x1F, 0x66, 0x1B, 0x66, 0x1B, 0x66, 0x9B, 0x66, 0x9B, 0x00, 0x00, 0x7C, 0x18, 0xFE, 0x18, 0xC7, 0x19, 0x83, 0x99, 0x83, 0x99, 0x83, 0xD9, 0x83, 0xD9, 0x83, 0x79, 0x83, 0x79, 0xC7, 0x39, 0xFE, 0x38, 0x7C, 0x18, 0x00, 0x00, 0x07, 0x7F, 0x8F, 0x7F, 0x9C, 0x61, 0x98, 0x61, 0x98, 0x61, 0x98, 0x7F, 0x18, 0x7F, 0x1B, 0x60, 0x1F, 0x60, 0x1C, 0x60, 0x0F, 0x60, 0x07, 0x60, 0x00, 0x00, 0xF0, 0xC7, 0xF8, 0xE7, 0x19, 0x76, 0x19, 0x36, 0x19, 0x36, 0xF8, 0x37, 0xF0, 0x37, 0x60, 0xB6, 0x70, 0xF6, 0x31, 0x76, 0x39, 0xE6, 0x18, 0xF6, 0x00, 0x30, 0x7F, 0x7E, 0x7F, 0xFF, 0x0C, 0xC3, 0x0C, 0x80, 0x0C, 0xC0, 0x0C, 0xFC, 0x0C, 0x7E, 0x0C, 0x07, 0x0C, 0x03, 0x0C, 0x87, 0x0C, 0xFE, 0x0C, 0xFC, 0x00, 0x00, 0x66, 0x98, 0x66, 0x98, 0x66, 0x18, 0x67, 0x18, 0x63, 0x18, 0x63, 0x18, 0x63, 0x18, 0x61, 0x18, 0x61, 0x18, 0x61, 0x18, 0xE0, 0x1F, 0xC0, 0x0F, 0x00, 0x00, 0x86, 0x0D, 0x86, 0x0D, 0xB6, 0x0D, 0xB6, 0x1D, 0xB6, 0x19, 0xB6, 0x19, 0xB6, 0xB9, 0xFC, 0xB0, 0xFC, 0xB0, 0xCC, 0xF0, 0xCC, 0xE0, 0xCC, 0xE0, 0x00, 0x00, 0x98, 0x61, 0x98, 0x61, 0x9C, 0x73, 0x0C, 0x33, 0x0F, 0x3F, 0x07, 0x1E, 0x07, 0x1E, 0x03, 0x3F, 0x03, 0x33, 0x83, 0x73, 0x83, 0x61, 0x83, 0x61, 0x00, 0x00, 0xF8, 0x67, 0xF8, 0x67, 0x38, 0xE0, 0x70, 0xC0, 0x60, 0xC0, 0xE0, 0x80, 0xC0, 0x81, 0x80, 0x01, 0x80, 0x03, 0x00, 0x07, 0xF8, 0x07, 0xF8, 0x07, 0x00, 0x00, 0x7C, 0xF8, 0x44, 0xF8, 0x44, 0xC0, 0x44, 0xC0, 0x7C, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0xC1, 0x07, 0xC3, 0x07, 0xC7, 0x00, 0xC6, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x07, 0xC0, 0x07, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0x38, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x00, 0x3F, 0x0F, 0x3F, 0x1F, 0x00, 0x18, 0x00, 0x18, 0x00, 0x1F, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0xF0, 0xC7, 0xF9, 0xE7, 0x19, 0x66, 0x19, 0xE6, 0x19, 0xE6, 0x19, 0x66, 0x19, 0x66, 0xF9, 0xE7, 0xF0, 0xE7, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x3F, 0xFC, 0x7F, 0xFE, 0x61, 0x86, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x61, 0x86, 0x7F, 0xFE, 0x3F, 0xFC, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x80, 0xC1, 0x8F, 0xE7, 0x9F, 0x67, 0x98, 0xE1, 0x9F, 0xE1, 0x9F, 0x01, 0x98, 0x61, 0x98, 0xE1, 0x9F, 0xC1, 0x8F, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF8, 0xFE, 0x98, 0xFE, 0x81, 0x8C, 0xF1, 0xFC, 0xF1, 0xF8, 0x80, 0x60, 0x80, 0xFC, 0x80, 0xFE, 0x81, 0x86, 0x81, 0xFE, 0x81, 0xFC, 0x00, 0x03, 0x60, 0x03, 0x60, 0x00, 0x60, 0x0F, 0x6F, 0x8F, 0x7F, 0x83, 0x71, 0x83, 0x61, 0x83, 0x61, 0x83, 0x61, 0x83, 0x61, 0x8F, 0x61, 0x8F, 0x61, 0x00, 0x00, 0x31, 0x00, 0x31, 0x00, 0x01, 0x00, 0xF1, 0x00, 0xF1, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31, 0xC0, 0xF1, 0xC1, 0xE0, 0x01, 0x3C, 0x80, 0x3C, 0x80, 0x0C, 0x80, 0x0C, 0x86, 0x0C, 0x8E, 0x0C, 0x9C, 0x0C, 0xB8, 0x0C, 0xF0, 0x0C, 0xF8, 0x0C, 0xDC, 0x3F, 0x8E, 0x3F, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0x36, 0xF7, 0x3F, 0xB7, 0x3B, 0x36, 0x33, 0x36, 0x33, 0x36, 0x33, 0x36, 0x33, 0x36, 0x33, 0x36, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF0, 0xFE, 0xF9, 0x86, 0x19, 0x86, 0x19, 0x86, 0x19, 0x86, 0x19, 0x86, 0x19, 0xFE, 0x19, 0xFC, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x7F, 0x9F, 0x7F, 0x98, 0x61, 0x98, 0x61, 0x98, 0x61, 0x9F, 0x7F, 0x0F, 0x7F, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xE3, 0xF9, 0xE3, 0x81, 0x63, 0x01, 0x63, 0x00, 0x63, 0x00, 0xE3, 0x01, 0xE3, 0x01, 0x63, 0x00, 0x63, 0x00, 0x60, 0x00, 0x00, 0x18, 0x00, 0x18, 0x00, 0x7F, 0xFC, 0x7F, 0xFE, 0x18, 0x86, 0x18, 0xF0, 0x18, 0xFC, 0x18, 0x1E, 0x18, 0x86, 0x1F, 0xFE, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x18, 0x66, 0x18, 0x66, 0x18, 0x63, 0x18, 0x63, 0x18, 0x61, 0x18, 0xE1, 0x18, 0xE0, 0x1F, 0x60, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x1B, 0x33, 0x1B, 0x33, 0x1B, 0x7B, 0x33, 0x7B, 0x33, 0xCE, 0xE1, 0xCE, 0xE1, 0x86, 0xC1, 0x86, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x98, 0x73, 0x0C, 0x3F, 0x0C, 0x1E, 0x07, 0x0C, 0x07, 0x1E, 0x03, 0x3F, 0x87, 0x73, 0x9E, 0x61, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x67, 0xF8, 0x67, 0x70, 0xC0, 0xE0, 0xC0, 0xC0, 0x81, 0x80, 0x83, 0x00, 0x07, 0xF8, 0x07, 0xF8, 0x07, 0x00, 0x00, 0xE0, 0x1C, 0xE0, 0x3C, 0xE0, 0x30, 0xE0, 0x30, 0xE0, 0x70, 0xE0, 0xE0, 0xE0, 0xC0, 0xE0, 0xE0, 0xE0, 0x70, 0xE0, 0x30, 0xE0, 0x30, 0xE0, 0x3C, 0x00, 0x1C, 0x00, 0x07, 0x80, 0x07, 0x80, 0x01, 0x80, 0x01, 0xC7, 0x01, 0xEF, 0x00, 0x6C, 0x00, 0xEC, 0x00, 0xC0, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x07, 0x00, 0x07, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x8C, 0x7F, 0xEC, 0x7F, 0xFC, 0x60, 0x38, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x03, 0x1E, 0x0F, 0x1E, 0x1E, 0x0E, 0x1C, 0x1E, 0x3B, 0x1C, 0x3F, 0x3C, 0xB8, 0x39, 0x9F, 0x3F, 0x0F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x01, 0xF0, 0x0B, 0x71, 0x9B, 0xE1, 0xF1, 0xC0, 0x61, 0x00, 0xF1, 0xE0, 0x99, 0xE1, 0x09, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E, 0x00, 0x9F, 0x73, 0x9B, 0xFF, 0x9B, 0xCC, 0x9B, 0xCC, 0x9B, 0xCC, 0x9F, 0xFF, 0x8E, 0x73, 0x00, 0x03, 0x0F, 0x83, 0x1F, 0x83, 0x19, 0x83, 0x19, 0x83, 0x1F, 0x03, 0x0F, 0x00, 0x80, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x80, 0xFA, 0x70, 0xDC, 0xF1, 0xF8, 0x80, 0x60, 0x00, 0xFC, 0x00, 0xCE, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x80, 0x00, 0xD8, 0x01, 0x9C, 0x03, 0x0F, 0x07, 0x07, 0x0E, 0x03, 0x1C, 0x07, 0x38, 0x0F, 0x70, 0xDC, 0xFF, 0xD8, 0xFF, 0x00, 0x00, 0xF8, 0xE7, 0xF8, 0xE7, 0x01, 0x00, 0x18, 0x66, 0x19, 0xE6, 0x31, 0xC3, 0x30, 0x83, 0xE0, 0x01, 0xE0, 0x81, 0xC0, 0xC0, 0xC1, 0xE1, 0x81, 0x67, 0x00, 0x07, 0x1C, 0x70, 0x3E, 0xF8, 0x77, 0xDC, 0x00, 0x00, 0x61, 0x86, 0x61, 0xCE, 0x33, 0xFC, 0x33, 0x78, 0x1E, 0x78, 0x1E, 0xFC, 0x0C, 0xCE, 0x7C, 0x86, 0x78, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x86, 0x00, 0xC6, 0x00, 0xE6, 0x80, 0xF6, 0xBF, 0xF6, 0x3F, 0xE6, 0x00, 0xC6, 0x00, 0x86, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x08, 0x18, 0x04, 0x18, 0xFE, 0x19, 0x01, 0x18, 0x01, 0x18, 0xFE, 0x19, 0x04, 0x18, 0x08, 0x18, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x18, 0x33, 0x19, 0x1E, 0x19, 0x0C, 0x19, 0x1E, 0x19, 0x33, 0x18, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xEC, 0xF0, 0xFD, 0xB0, 0xBD, 0xB0, 0xBD, 0xF1, 0xFD, 0xE1, 0xEC, 0x00, 0x00, 0x01, 0x00, 0x07, 0x00, 0x1F, 0x00, 0x7C, 0x06, 0x70, 0x06, 0x7C, 0x06, 0x1F, 0x06, 0x07, 0x06, 0x01, 0x06, 0x00, 0x06, 0x7F, 0xFE, 0x7F, 0xFE, 0x00, 0x00, 0x87, 0xC1, 0x87, 0xC1, 0x81, 0x01, 0xF0, 0x3F, 0xF0, 0x3F, 0x00, 0x03, 0x01, 0x03, 0xF7, 0xFF, 0xF7, 0xFF, 0x00, 0x06, 0x07, 0xC6, 0x07, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC0, 0x44, 0xF0, 0x44, 0x7C, 0x44, 0x1C, 0xC7, 0x7D, 0x01, 0xF1, 0x82, 0xC0, 0x44, 0x00, 0x28, 0x00, 0x10, 0xFC, 0x00, 0xFC, 0x00, 0x00, 0xC0, 0x03, 0xC1, 0x03, 0x01, 0x03, 0x01, 0x07, 0x01, 0xE6, 0x01, 0xE6, 0x03, 0x6E, 0x03, 0x7C, 0x03, 0x3C, 0x03, 0x3C, 0x1B, 0x18, 0x1F, 0x18, 0x0E, 0x00, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0xB0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0xC0, 0x80, 0xE0, 0x01, 0xE0, 0x01, 0x30, 0x03, 0xF0, 0x03, 0xF8, 0x07, 0x18, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xF8, 0x7F, 0xFC, 0x63, 0xCC, 0x60, 0xF8, 0x63, 0xCC, 0x7F, 0xFC, 0x3E, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x87, 0x1B, 0xC7, 0x1F, 0xC6, 0x1C, 0xC0, 0x18, 0xC0, 0x18, 0xC0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1E, 0x80, 0x7E, 0xE0, 0xFE, 0xF9, 0x7E, 0xE0, 0x1E, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1E, 0x0F, 0x3F, 0x0F, 0x33, 0x03, 0x33, 0x03, 0x33, 0x03, 0x3F, 0x03, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 0xF0, 0x03, 0x30, 0x00, 0xE0, 0x00, 0x80, 0x01, 0xF0, 0x03, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xF8, 0x1E, 0xFC, 0x16, 0x0C, 0x36, 0x78, 0x3F, 0x0C, 0x3F, 0xFC, 0x06, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC1, 0x0F, 0xC3, 0x0F, 0x03, 0x0C, 0x83, 0x0F, 0xC3, 0x00, 0xC3, 0x0F, 0x81, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xE0, 0xFC, 0xE0, 0x0C, 0x00, 0x18, 0xE0, 0x18, 0x30, 0x30, 0xF0, 0x30, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1E, 0x0F, 0x3F, 0x0C, 0x33, 0x07, 0x1E, 0x00, 0x33, 0x07, 0x3F, 0x07, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x78, 0xC0, 0x79, 0xC0, 0x99, 0xCF, 0x98, 0xCF, 0x19, 0xC0, 0x19, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x00, 0xEF, 0xEC, 0xEC, 0xBC, 0x6C, 0x30, 0x6C, 0x64, 0x6F, 0xEC, 0x67, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x80, 0x83, 0x80, 0x07, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x80, 0x07, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xE0, 0x0C, 0xF0, 0x3F, 0x30, 0x0C, 0x30, 0x0C, 0x30, 0x00, 0xF0, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1E, 0x0F, 0x3F, 0x0F, 0x33, 0x03, 0x33, 0x03, 0x33, 0x03, 0x3F, 0x03, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 0xF0, 0x03, 0x30, 0x00, 0xE3, 0x00, 0x83, 0x01, 0xF0, 0x03, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x06, 0x3F, 0x1E, 0x30, 0x1E, 0x3E, 0xE6, 0x3E, 0xE6, 0x30, 0x06, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x18, 0xC3, 0x1C, 0xC3, 0x1E, 0xC3, 0x1F, 0xC3, 0x1B, 0xC3, 0x19, 0xC3, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0xE0, 0x66, 0xF0, 0xCC, 0x30, 0xCC, 0xF0, 0xFC, 0xE0, 0xB6, 0x01, 0x80, 0x01, 0x3F, 0x1E, 0x3F, 0x1E, 0x3C, 0x3F, 0x3C, 0x3F, 0xBC, 0x7F, 0xBF, 0x67, 0xBF, 0x67, 0xFC, 0xFF, 0xFC, 0xFF, 0xFC, 0xC3, 0xFF, 0xC3, 0xFF, 0xC3, 0x00, 0x00, 0xFB, 0xE1, 0xFF, 0xF7, 0x8F, 0x37, 0x03, 0x3F, 0x03, 0x3F, 0x03, 0xEF, 0x03, 0xEF, 0x03, 0x3F, 0x03, 0x3F, 0x8F, 0x37, 0xFF, 0xF7, 0xFB, 0xE1, 0x00, 0x00, 0x7F, 0xFC, 0x7F, 0xFE, 0x78, 0xC7, 0x78, 0xC3, 0x78, 0xC3, 0x7F, 0xC3, 0x7F, 0xC3, 0x78, 0xC3, 0x78, 0xC3, 0x78, 0xC7, 0x7F, 0xFE, 0x7F, 0xFC, 0x00, 0x00, 0xFF, 0xDF, 0xFF, 0xDF, 0x0F, 0x1E, 0x0F, 0x1E, 0x0F, 0x1E, 0xEF, 0x9F, 0xEF, 0x9F, 0x0F, 0x1E, 0x0F, 0x1E, 0x0F, 0x1E, 0x0F, 0xDE, 0x0F, 0xDE, 0x00, 0x00, 0x00, 0xF8, 0x00, 0xFC, 0x00, 0x0C, 0x80, 0x0D, 0x60, 0x0D, 0x18, 0xFD, 0x06, 0xF9, 0x18, 0x01, 0x60, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xC0, 0xFF, 0xC0, 0x70, 0x0F, 0x38, 0x1F, 0x1C, 0x39, 0x0E, 0x30, 0x07, 0x30, 0x0E, 0x30, 0x1C, 0x39, 0x38, 0xDF, 0x70, 0xCF, 0xFF, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x3E, 0x7C, 0xBF, 0xF0, 0xE3, 0xC0, 0xE1, 0x80, 0xC3, 0x01, 0xE3, 0x01, 0xE7, 0x01, 0xB6, 0x00, 0x36, 0x00, 0x06, 0x0F, 0x00, 0x1F, 0x00, 0x39, 0x00, 0x30, 0x7E, 0x30, 0xFE, 0x7F, 0xC0, 0x7F, 0x78, 0x61, 0xF8, 0x61, 0xC0, 0x73, 0x80, 0x3F, 0xFC, 0x1E, 0xFC, 0x00, 0x00, 0x00, 0x07, 0x80, 0x87, 0x80, 0xC1, 0x81, 0xC1, 0x81, 0xC3, 0x81, 0xC3, 0xC3, 0xC7, 0xC3, 0x86, 0xC3, 0x8E, 0xC7, 0x8C, 0xF6, 0x1C, 0x76, 0x18, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x8D, 0xFF, 0x8F, 0xCC, 0x8C, 0xCC, 0x9C, 0xDC, 0x19, 0x98, 0x39, 0x98, 0xF9, 0x9E, 0xDB, 0x0E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x1F, 0xCF, 0x3F, 0x1D, 0x73, 0x99, 0x61, 0x9B, 0xE3, 0x1B, 0xC3, 0x1F, 0xE7, 0x0F, 0x7E, 0x06, 0x3C, 0x06, 0x00, 0x71, 0xC0, 0xF9, 0xC0, 0xDB, 0xC1, 0x9B, 0xE1, 0x99, 0xF3, 0x39, 0xB3, 0x31, 0xB3, 0x71, 0x33, 0xE1, 0x71, 0xC1, 0xE3, 0xED, 0xCF, 0x7D, 0x0E, 0x38, 0x00, 0xFF, 0x9C, 0xFF, 0xFC, 0xFF, 0xF0, 0xFF, 0x80, 0xFF, 0xBC, 0xFF, 0xFE, 0xFF, 0xC6, 0xFF, 0x86, 0xFF, 0x86, 0xFF, 0x86, 0xFF, 0x86, 0xFF, 0x86, 0xFF, 0x00, 0xF0, 0xFF, 0xF0, 0xFF, 0x30, 0xF0, 0x30, 0xF0, 0x30, 0xF0, 0x30, 0xF0, 0x33, 0xF0, 0x33, 0xF0, 0x30, 0xF0, 0x31, 0xF0, 0x30, 0xF0, 0xF3, 0xFF, 0xF3, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0x30, 0x00, 0xE0, 0xFF, 0x33, 0xFF, 0xF3, 0xFF, 0xE3, 0x80, 0x0F, 0xC0, 0x1F, 0xC0, 0x1C, 0x1F, 0x1C, 0x9F, 0x7F, 0x9C, 0x7F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x1C, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE6, 0xF8, 0xF7, 0xB8, 0x77, 0x38, 0x77, 0x38, 0x77, 0x38, 0xF7, 0x39, 0xE7, 0x39, 0x07, 0x39, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x77, 0xFF, 0x77, 0xED, 0x77, 0xED, 0xEE, 0xED, 0xEE, 0xED, 0xFF, 0xED, 0xF7, 0xED, 0xC0, 0xED, 0xC0, 0x00, 0xC0, 0x0E, 0x1C, 0x0E, 0x1C, 0x0F, 0x1C, 0x7F, 0x9C, 0xFF, 0xDC, 0xEF, 0xDD, 0xCF, 0xDF, 0x8E, 0xDF, 0xCE, 0xDF, 0xEE, 0xDD, 0xFE, 0xDC, 0x7E, 0xDC, 0x00, 0x00, 0x7E, 0x1C, 0xFF, 0x1D, 0xE3, 0x3D, 0xC0, 0x3F, 0xC0, 0x3F, 0xCF, 0xFF, 0xCF, 0xFF, 0xC3, 0xDF, 0xC3, 0xDF, 0xE3, 0xDD, 0xFF, 0x1D, 0x7E, 0x1C, 0x00, 0x00, 0xDF, 0x7F, 0xDF, 0x7F, 0x1C, 0x0E, 0x1C, 0x0E, 0x1C, 0x0E, 0x1F, 0x0E, 0x1F, 0x0E, 0x1C, 0x0E, 0x1C, 0x0E, 0x1C, 0x0E, 0x1C, 0x0E, 0x1C, 0x0E, 0x00, 0x00, 0xF8, 0xE7, 0xF8, 0xF7, 0x00, 0x77, 0x00, 0x77, 0x00, 0x77, 0xF0, 0xF7, 0xF0, 0xE7, 0x00, 0x07, 0x01, 0x07, 0x01, 0x07, 0xF9, 0x07, 0xF9, 0x07, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x60, 0x00, 0xE0, 0x00, 0xC0, 0x00, 0xFE, 0x00, 0xFE, 0x3E, 0xC0, 0x3F, 0xC0, 0x33, 0x80, 0x3F, 0x80, 0x3E, 0x80, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x19, 0x86, 0x1F, 0x07, 0x18, 0x83, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0x60, 0xDC, 0x60, 0xF8, 0x60, 0xF0, 0x60, 0xF8, 0x60, 0xDC, 0xE0, 0xCE, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x3F, 0x0C, 0x4C, 0x0C, 0x0C, 0x8F, 0x0F, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x01, 0xF0, 0x03, 0x30, 0xC3, 0xF1, 0xC1, 0x31, 0xC0, 0xF1, 0xC1, 0xE0, 0x61, 0x00, 0x38, 0x00, 0x7C, 0x00, 0x6C, 0x00, 0x38, 0x00, 0x7C, 0x00, 0x6C, 0x00, 0xEE, 0x00, 0xC6, 0xBB, 0xFE, 0xFF, 0xFF, 0xCC, 0x83, 0xCC, 0x83, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x01, 0x0F, 0x01, 0x86, 0x03, 0xC6, 0x03, 0xC6, 0x03, 0xCF, 0x00, 0xCF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x60, 0x00, 0x60, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF1, 0x09, 0xFB, 0x0B, 0x1C, 0x0F, 0x0C, 0x0E, 0x8C, 0x0F, 0x0C, 0x00, 0x1C, 0xF3, 0xF8, 0xF3, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x60, 0x7F, 0xF0, 0x73, 0xF8, 0x63, 0xFC, 0x63, 0x60, 0x63, 0x63, 0x63, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x7F, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x0E, 0xC0, 0x0F, 0xF0, 0x0F, 0xC0, 0x0F, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xE1, 0x07, 0xE7, 0x1F, 0xE1, 0x07, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x00, 0xFC, 0x00, 0xFF, 0x00, 0xFC, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x38, 0x00, 0x00, 0x00};

void draw_glyph(byte x, byte y, byte chr)
{
	const auto width = 10;
	const auto height = 13;
	auto wi = width * (byte)chr;
	auto index2 = (wi >> 4);
	auto n = wi & 15;
	auto n2 = (16 - n);
	if (n2 > width)
		n2 = width;
	auto buf = (byte __near *)GetScreenBuffer();
	auto buf2 = (byte __near *)GetScreenBuffer() + 0x600;
	auto bs = (ushort *)normal_font + index2 * (height - 1);
	for (int j = 0; j < height; j++)
	{
		ushort dat = (((bs[index2]) << n) | ((bs[index2 + height]) >> n2)) & 0b1111111111000000;
		byte dat1 = (dat) >> ((x & 7) + 8);
		byte dat2 = (dat) >> ((x & 7));
		if (Color & 1)
		{
			buf[(j + 1 + y) * 24 + (x >> 3)] ^= dat1;
			buf[(j + 1 + y) * 24 + (x >> 3) + 1] ^= dat2;
		}
		if (Color & 2)
		{
			buf2[(j + 1 + y) * 24 + (x >> 3)] ^= dat1;
			buf2[(j + 1 + y) * 24 + (x >> 3) + 1] ^= dat2;
		}
		index2++;
	}
}
void line_print_n(const char __near *str, byte x, byte y)
{
	while (1)
	{
		byte c = (byte) * (str++);
		if (!c)
			break;
		draw_glyph(x, y, c - 0x10);
		x += 11;
	}
}