/*============================================================================
-   yfuns.h

-

-   Copyright (C) 2008-2021 LAPIS Technology Co., Ltd.

============================================================================ */
#pragma once
#include "base.h"

/* process control functions */
#define _Exit(status) _exit(status)

/* stdio functions */
#define _Fclose(str) close((str)->_Handle)
#define _Fread(str, buf, cnt) read((str)->_Handle, buf, cnt)
#define _Fwrite(str, buf, cnt) write((str)->_Handle, buf, cnt)

/* interface declarations */

int close(int);
void _exit(int);

int read(int, unsigned char __near *, unsigned int);
int write(int, unsigned char __near *, unsigned int);