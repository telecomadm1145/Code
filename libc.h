#pragma once
#include "libc/stdlib.h"
#include "libc/math.h"
#include "libc/string.h"
#include "libc/ctype.h"
using byte = unsigned char;
using ushort = unsigned short;
using ulong = unsigned long;
using uint16_t = unsigned short;
extern "C"
{
    void DebugOutputString(const char *);
    void memzero_n(void __near *, ushort);
    byte get_msb(byte);
}

template <typename X>
ushort itoa_n(X value, char __near *ptr)
{
    if (value == 0){
        ptr[0] = '0';
        ptr[1] = 0;
        return 1;
    }
    bool isNegative = value < 0;
    if (isNegative)
    {
        value = -value;
        *ptr++ = '-';
    }
    X temp = value;
    ushort numDigits = 0;
    do
    {
        temp /= 10;
        numDigits++;
    } while (temp > 0);
    ptr[numDigits] = '\0';
    for (int i = numDigits - 1; i >= 0; --i)
    {
        ptr[i] = (value % 10) + '0';
        value /= 10;
    }
    return numDigits;
}
/*
template <typename X>
X atoi_n(const char __near *str)
{
    // Handle optional sign
    bool isNegative = false;
    if (*str == '-')
    {
        isNegative = true;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    // Convert characters to integer
    X result = 0;
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return isNegative ? -result : result;
}
template <typename X>
X atoi_f(const char *str)
{
    // Handle optional sign
    bool isNegative = false;
    if (*str == '-')
    {
        isNegative = true;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    // Convert characters to integer
    X result = 0;
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return isNegative ? -result : result;
}

*/
inline void DebugOutputInt(long n)
{
    char data[20];
    itoa_n(n, (char __near*)data);
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
