/*============================================================================
-   math.h

-   Defines the math macros and functions.

-   Copyright (C) 2008-2021 LAPIS Technology Co., Ltd.

============================================================================ */

#pragma once
#include "base.h"
extern "C"
{
	float acosf(float);
	float asinf(float);
	float atanf(float);
	float atan2f(float, float);
	float ceilf(float);
	float cosf(float);
	float coshf(float);
	float expf(float);
	float fabsf(float);
	float floorf(float);
	float fmodf(float, float);
	float frexp(float, int *);
	float ldexpf(float, int);
	float logf(float);
	float log10f(float);
	float modf(float, float *);
	float powf(float, float);
	float sinf(float);
	float sinhf(float);
	float sqrtf(float);
	float tanf(float);
	float tanhf(float);
}