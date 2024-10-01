/*============================================================================
-   stddef.h

-   Defines the stddef macros.

-   Copyright (C) 2008-2021 LAPIS Technology Co., Ltd.

============================================================================ */
/* stddef.h standard header */
#pragma once
#include "base.h"
#define offsetof(T, member) ((_Sizet) & ((T *)0)->member)