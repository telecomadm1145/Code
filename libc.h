using byte = unsigned char;
using ushort = unsigned short;
#ifdef __INTELLISENSE__
#define __near
#define __DI()
#define __EI()
#endif
extern "C" {
    void __near* memcpy_nn(void __near*,void __near*,ushort);
}