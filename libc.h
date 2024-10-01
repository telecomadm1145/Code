#pragma once
#include "libc/stdlib.h"
#include "libc/math.h"
#include "libc/string.h"
using byte = unsigned char;
using ushort = unsigned short;
using ulong = unsigned long;
extern "C"
{
    void DebugOutputString(const char *);
        void memzero_n(void __near *, ushort);
            byte get_msb(byte);
}

inline void DebugOutputInt(long n)
{
    char data[20];
    itoa(n, data,10);
    DebugOutputString(data);
    DebugOutputString("\n");
}
template <typename TNumber>
TNumber min(TNumber a, TNumber b)
{
    return a > b ? b : a;
}
template <typename TNumber>
TNumber max(TNumber a, TNumber b)
{
    return a < b ? b : a;
}

inline ushort byteswap_16(ushort x)
{
    return (x >> 8) | (x << 8);
}
inline ulong byteswap_32(ulong x)
{
    return (x >> 24) | ((x >> 8) & 0xff) | ((x << 8) & 0xff) | (x << 24);
}
