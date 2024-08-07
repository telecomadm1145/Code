
#include "libc.h"
#include "classwiz_bsp.h"

class MenuItem
{
public:
    ushort op;
    const char *string;
};


const MenuItem test_menu[] = {
    {0, "MenuItem1"},
    {1, "MenuItem2"},
    {2, "MenuItem3"},
    {3, "MenuItem4"},
    {4, "MenuItem5"},
    {5, "MenuItem6"},
    {6, "MenuItem7"},
    {7, "MenuItem8"},
    {8, "MenuItem9"},
    {9, "MenuItem10"},
    {10, "MenuItem11"},
    {11, "MenuItem12"},
    {12, "MenuItem13"},
    {13, "MenuItem14"},
};


ushort show_menu(const MenuItem *pmenuitems, byte count);