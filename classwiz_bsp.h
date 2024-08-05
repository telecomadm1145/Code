#include "libc.h"
#define val(x) *((volatile __near byte *)x)
#define StopAcceptor *((volatile __near byte *)0xF008)
#define StopControl *((volatile __near byte *)0xF009)
#define FCON *((volatile __near byte *)0xF00A)
#define InterruptMask0 *((volatile __near byte *)0xF010)
#define InterruptMask1 *((volatile __near byte *)0xF011)
#define InterruptMask_W0 *((volatile __near ushort *)0xF010)
#define InterruptMask2 *((volatile __near byte *)0xF012)
#define InterruptMask3 *((volatile __near byte *)0xF013)
#define InterruptMask_W1 *((volatile __near ushort *)0xF012)
#define InterruptPending0 *((volatile __near byte *)0xF014)
#define InterruptPending1 *((volatile __near byte *)0xF015)
#define InterruptPending_W0 *((volatile __near ushort *)0xF014)
#define InterruptPending2 *((volatile __near byte *)0xF016)
#define InterruptPending3 *((volatile __near byte *)0xF017)
#define InterruptPending_W1 *((volatile __near ushort *)0xF016)
#define Timer0Interval *((volatile __near ushort *)0xF020)
#define Timer0Counter *((volatile __near ushort *)0xF022)
#define Timer0Control *((volatile __near ushort *)0xF024)
#define Timer0Control0 *((volatile __near byte *)0xF024)
#define Timer0Control1 *((volatile __near byte *)0xF025)
#define BlockControl *((volatile __near byte *)0xF028)
#define ScreenRange *((volatile __near byte *)0xF030)
#define ScreenMode *((volatile __near byte *)0xF031)
#define ScreenContrast *((volatile __near byte *)0xF032)
#define ScreenBrightness *((volatile __near byte *)0xF033)
#define ScreenInterval *((volatile __near byte *)0xF034)
#define ScreenUnk1 *((volatile __near byte *)0xF035)
#define ScreenUnk2 *((volatile __near byte *)0xF036)
#define ScreenSelect *((volatile __near byte *)0xF037)
#define ScreenOffset *((volatile __near byte *)0xF039)
#define ScreenPower *((volatile __near byte *)0xF03D)

void reset_timer();
void delay(ushort after_ticks);
void reset_sfrs();