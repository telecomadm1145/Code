@echo off

SET PATH=C:\LAPIS\LEXIDE\Bin;C:\LAPIS\LEXIDE\BuildTools\Ver.20231124\Bin;C:\LAPIS\LEXIDE\Utilities\Bin;%PATH%

clang-u16.exe -c test.cpp -O3 -v
clang-u16.exe -c classwiz_bsp.cpp -O3 -v
llvm-mc-u16 startup.asm
llvm-mc-u16 line_print.asm
llvm-mc-u16 libc.asm
lld-u16 -S -L"C:/LAPIS/LEXIDE/BuildTools/Ver.20231124/Lib/U16/" -T startup.ld -o program.elf -Map program.map startup.o libc.o test.o classwiz_bsp.o line_print.o -l"PL99U16SWNPK.a" -l"longu8.a" -l"floatu8.a"
if %ERRORLEVEL% NEQ 0 (
    exit
)
llvm-objcopy.exe --output-target=ihex program.elf program.hex
if %ERRORLEVEL% NEQ 0 (
    exit
)
hex2bin.exe program.hex

copy program.bin "C:\Users\15874\source\repos\CasioEmuX\x64\Release\models\Cosine\Cosine.bin"
cd C:\Users\15874\source\repos\CasioEmuX\x64\Release
CasioEmuMsvc.exe models\Cosine