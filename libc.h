using byte = unsigned char;
using ushort = unsigned short;
#ifdef __INTELLISENSE__
#define __near
#define __far
#define __DI()
#define __EI()
#endif
extern "C"
{
    void __near *memcpy_nn(void __near *, void __near *, ushort);
    void memzero_n(void __near *, ushort);
    void memzero_w_n(void __near *, ushort);
    void memset_n(void __near *, byte, ushort);
}