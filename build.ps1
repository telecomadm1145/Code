# 设置路径
$path = "C:\LAPIS\LEXIDE\Bin;C:\LAPIS\LEXIDE\BuildTools\Ver.20231124\Bin;C:\LAPIS\LEXIDE\Utilities\Bin;$env:PATH"
$env:PATH = $path

# 查找所有的 C/C++ 文件和 ASM 文件
$cFiles = Get-ChildItem -Recurse -Filter *.c | ForEach-Object { $_.FullName }
$cppFiles = Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
$asmFiles = Get-ChildItem -Recurse -Filter *.asm | ForEach-Object { $_.FullName }

& clang-u16.exe -c ($cFiles + $cppFiles) -O3 -v
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 编译 ASM 文件
foreach ($file in $asmFiles) {
    & llvm-mc-u16 $file
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

# 查找所有生成的对象文件
$objFiles = Get-ChildItem -Recurse -Filter *.o | ForEach-Object { $_.FullName }

# 链接
& lld-u16 --print-gc-sections -S -L"C:/LAPIS/LEXIDE/BuildTools/Ver.20231124/Lib/U16/" -T startup.ld -o program.elf -Map program.map $objFiles -l"PL99U16LWF.a" -l"longu8.a" -l"doubleu8.a"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 转换 ELF 文件为 HEX 文件
& llvm-objcopy.exe --output-target=ihex program.elf program.hex
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 转换 HEX 文件为 BIN 文件
& ./hex2bin.exe program.hex
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# 复制 BIN 文件到指定目录
# Copy-Item -Force program.bin "C:\Users\15874\source\repos\CasioEmuX\x64\PatchableRelease\models\Cosine\Cosine.bin"

# 定义文件路径
$programPath = "program.bin"
$fontPath = "font.bin"
$indexPath = "index.bin"
$outputPath = "C:\Users\15874\source\repos\CasioEmuX\x64\PatchableRelease\models\Cosine\Cosine.bin"

# 创建一个新的空文件，大小为 0x60000
$size = 0x60000
$emptyFile = New-Object byte[] $size

# 读取 program.bin 的内容
$programBytes = [System.IO.File]::ReadAllBytes($programPath)
$programBytes.CopyTo($emptyFile,0)

# 读取 font.bin 和 index.bin 的内容
$fontBytes = [System.IO.File]::ReadAllBytes($fontPath)
$indexBytes = [System.IO.File]::ReadAllBytes($indexPath)

# 将 font.bin 写入 output.bin 的 0x10000 处
$fontOffset = 0x10000
$fontBytes.CopyTo($emptyFile,0x10000)

# 将 index.bin 写入 output.bin 的 0x50000 处
$indexOffset = 0x50000
$indexBytes.CopyTo($emptyFile,0x50000)

[System.IO.File]::WriteAllBytes($outputPath,$emptyFile)

Write-Host "操作完成，输出文件为: $outputPath"

# 删除临时对象文件
Remove-Item -Force $objFiles

# 切换到指定目录并运行 CasioEmuMsvc.exe
& "cmd.exe" /c cd C:\Users\15874\source\repos\CasioEmuX\x64\PatchableRelease `&`& CasioEmuMsvc.exe models\Cosine