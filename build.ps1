# 设置路径
$path = "C:\LAPIS\LEXIDE\Bin;C:\LAPIS\LEXIDE\BuildTools\Ver.20231124\Bin;C:\LAPIS\LEXIDE\Utilities\Bin;$env:PATH"
$env:PATH = $path

# 查找所有的 C/C++ 文件和 ASM 文件
$cFiles = Get-ChildItem -Recurse -Filter *.c | ForEach-Object { $_.FullName }
$cppFiles = Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
$asmFiles = Get-ChildItem -Recurse -Filter *.asm | ForEach-Object { $_.FullName }

# 编译 C/C++ 文件
foreach ($file in $cFiles + $cppFiles) {
    & clang-u16.exe -c $file -O3 -v
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    & clang-u16.exe -c $file -O3 -S
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

# 编译 ASM 文件
foreach ($file in $asmFiles) {
    & llvm-mc-u16 $file
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

# 查找所有生成的对象文件
$objFiles = Get-ChildItem -Recurse -Filter *.o | ForEach-Object { $_.FullName }

# 链接
& lld-u16 -S -L"C:/LAPIS/LEXIDE/BuildTools/Ver.20231124/Lib/U16/" -T startup.ld -o program.elf -Map program.map $objFiles -l"PL99U16SWNPK.a" -l"longu8.a" -l"floatu8.a"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 转换 ELF 文件为 HEX 文件
& llvm-objcopy.exe --output-target=ihex program.elf program.hex
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 转换 HEX 文件为 BIN 文件
& ./hex2bin.exe program.hex
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 复制 BIN 文件到指定目录
Copy-Item -Force program.bin "C:\Users\15874\source\repos\CasioEmuX\x64\Release\models\Cosine\Cosine.bin"

# 删除临时对象文件
Remove-Item -Force $objFiles

# 切换到指定目录并运行 CasioEmuMsvc.exe
& "cmd.exe" /c cd C:\Users\15874\source\repos\CasioEmuX\x64\Release `&`& CasioEmuMsvc.exe models\Cosine