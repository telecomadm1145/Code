/*============================================================================
-   errno.h

-   Defines the errno macros.

-   Copyright (C) 2008-2021 LAPIS Technology Co., Ltd.

============================================================================ */
/* errno.h standard header */
#pragma once
#include "base.h"

/* error codes */
#define EDOM _EDOM
#define ERANGE _ERANGE
#define EFPOS _EFPOS
/* ADD YOURS HERE */
#define _NERR _ERRMAX /* one more than last code */
					  /* declarations */
extern volatile int errno;