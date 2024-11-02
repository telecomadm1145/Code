#include "base.h"
#include "classwiz_bsp.h"
byte sd_init() ;
byte sd_read_block(unsigned long addr, byte* buffer);
byte sd_write_block(unsigned long addr, const byte* buffer);
byte sd_get_info(unsigned long* capacity);