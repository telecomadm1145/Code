/*============================================================================
-   stdarg.h

-   Defines the stdarg macros

-   Copyright (C) 2008-2021 LAPIS Technology Co., Ltd.

============================================================================ */
/* stdarg.h standard header */

#pragma once
#include "base.h"

#ifdef __LCCU16__

#ifndef _VA_LIST
typedef __builtin_va_list va_list;
#define _VA_LIST
#endif
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

#else
/* type definitions */

typedef char __near *va_list;

/* macros */

#define va_arg(ap, T) \
	((ap) += _Bnd(T, _AUPBND), *(T __near *)((ap) - _Bnd(T, _ADNBND)))
#define va_end(ap) (void)0
#define va_start(ap, A) \
	(void)((ap) = (char __near *)&(A) + _Bnd(A, _AUPBND))
#define _Bnd(X, bnd) (sizeof(X) + (bnd) & ~(bnd))

#endif /*__LCCU16__*/