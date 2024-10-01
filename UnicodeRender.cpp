#include "UnicodeRender.h"
unsigned long GetGlyphOffset(ushort d, bool __near &is_8)
{
    // 0x5C200
    // 右移三位的压缩指针，我们要左移三位修复
    auto cptr = ((ushort *)0x5C400)[d >> 8]; // 第一级指针
    if (cptr == 0xffff)                      // INVALID
    {
        is_8 = true;
        return (unsigned int)0x3D340;
    }
    volatile auto ptr1 = (ushort *)((byte *)0x50000 + (cptr << 3));
    volatile auto ptr2 = ptr1[(d & 255)];
    is_8 = (ptr2 & 0x8000) >> 15;                                                        // 是否为8位
    volatile auto ptr3 = (unsigned long)0x10000 + (((unsigned long)ptr2 & 0x7fff) << 4); // 解压的指针
    return ptr3;
}

bool DrawGlyph2(byte x, byte y, ushort chr)
{
    bool is_8 = false;
    auto glo = GetGlyphOffset(chr, (bool __near &)is_8);
    if (is_8)
    {
        auto base = (byte __near *)(0xf800 + y * 32 + (x >> 3));
        for (byte i = 0; i < 16; i++)
        {
            *base |= *(byte __far *)(glo + 1);
            glo += 2;
            base += 0x20;
        }
    }
    else
    {
        auto base = (byte __near *)(0xf800 + y * 32 + (x >> 3));
        for (byte i = 0; i < 16; i++)
        {
            *base |= *((byte __far *)glo);
            *(base + 1) |= *((byte __far *)(glo + 1));
            glo += 2;
            base += 0x20;
        }
    }
    return is_8;
}
