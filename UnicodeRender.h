#include "libc.h"
inline ushort ReadUtf8(char __near *utf8String, ushort __near &codeLength)
{
    ushort unicodeValue = 0;
    constexpr int i = 0;
    // Check the number of leading 1s to determine the length of the UTF-8 code
    if ((utf8String[i] & 0x80) == 0)
    {
        // Single-byte character (ASCII)
        unicodeValue = utf8String[i];
        codeLength = 1;
    }
    else if ((utf8String[i] & 0xE0) == 0xC0)
    {
        // Two-byte character
        unicodeValue = (utf8String[i] & 0x1F) << 6;
        unicodeValue |= (utf8String[i + 1] & 0x3F);
        codeLength = 2;
    }
    else if ((utf8String[i] & 0xF0) == 0xE0)
    {
        // Three-byte character
        unicodeValue = (utf8String[i] & 0x0F) << 12;
        unicodeValue |= (utf8String[i + 1] & 0x3F) << 6;
        unicodeValue |= (utf8String[i + 2] & 0x3F);
        codeLength = 3;
    }
    /*else if ((utf8String[i] & 0xF8) == 0xF0)
    {
        // Four-byte character
        unicodeValue = (utf8String[i] & 0x07) << 18;
        unicodeValue |= (utf8String[i + 1] & 0x3F) << 12;
        unicodeValue |= (utf8String[i + 2] & 0x3F) << 6;
        unicodeValue |= (utf8String[i + 3] & 0x3F);
        codeLength = 4;
    }*/
    else
    {
        unicodeValue = '?';
        codeLength = 1;
    }
    return unicodeValue;
}
bool DrawGlyph2(byte x, byte y, ushort chr);
inline void DrawTextWithWrapping(const wchar_t __near *text, int startX, int startY)
{
    int x = startX;
    int y = startY;
    const wchar_t __near *d = text;

    while (*d != 0)
    {
        if (x + 16 > 192)
        {
            x = startX;
            y += 16;
        }
        if (*d == '\n' || *d == '\r')
        {
            if (x != startX)
            {
                x = startX;
                y += 16;
            }
            d++;
            continue;
        }
        if (DrawGlyph2(x, y, *d))
        {
            x += 8;
        }
        else
        {
            x += 16;
        }
        d++;
    }
}
