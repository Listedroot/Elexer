@echo off
setlocal enabledelayedexpansion

:: Set up paths
set ROOT=%~dp0
set KERNEL_BIN=%ROOT%kernel.bin
set KERNEL_IMG=%ROOT%kernel.img
set BOOTLOADER=%ROOT%boot\boot.bin
set TEMP_KERNEL=%TEMP%\temp_kernel.bin

:: Clean previous build
echo Cleaning previous build...
if exist "%KERNEL_BIN%" del /f /q "%KERNEL_BIN%"
if exist "%KERNEL_IMG%" del /f /q "%KERNEL_IMG%"
if exist "%TEMP_KERNEL%" del /f /q "%TEMP_KERNEL%"

:: Compile bootloader
echo Building bootloader...
if not exist "%ROOT%boot\boot.s" (
    echo Error: boot.s not found in boot directory
    exit /b 1
)

nasm -f bin "%ROOT%boot\boot.s" -o "%BOOTLOADER%"
if %ERRORLEVEL% NEQ 0 (
    echo Error building bootloader
    exit /b 1
)

:: Compile libc first
echo Compiling libc...
if exist "%ROOT%libc" (
    for /r "%ROOT%libc" %%f in (*.c) do (
        echo Compiling %%~nxf
        gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-common -fno-pic -Wall -Wextra -Werror -I"%ROOT%include" -c "%%f" -o "%%~dpnf.o"
        if !ERRORLEVEL! NEQ 0 exit /b !ERRORLEVEL!
    )
)

:: Compile kernel objects
echo Compiling kernel...
if exist "%ROOT%kernel" (
    for /r "%ROOT%kernel" %%f in (*.c) do (
        echo Compiling %%~nxf
        gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-common -fno-pic -Wall -Wextra -Werror -I"%ROOT%include" -c "%%f" -o "%%~dpnf.o"
        if !ERRORLEVEL! NEQ 0 exit /b !ERRORLEVEL!
    )

    for /r "%ROOT%kernel" %%f in (*.s) do (
        echo Assembling %%~nxf
        nasm -f win32 "%%f" -o "%%~dpnf.o"
        if !ERRORLEVEL! NEQ 0 exit /b !ERRORLEVEL!
    )
)

:: Compile drivers
echo Compiling drivers...
if exist "%ROOT%drivers" (
    for /r "%ROOT%drivers" %%f in (*.c) do (
        echo Compiling %%~nxf
        gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-common -fno-pic -Wall -Wextra -Werror -I"%ROOT%include" -c "%%f" -o "%%~dpnf.o"
        if !ERRORLEVEL! NEQ 0 exit /b !ERRORLEVEL!
    )
)

:: Compile libc
echo Compiling libc...
if exist "%ROOT%libc" (
    for /r "%ROOT%libc" %%f in (*.c) do (
        echo Compiling %%~nxf
        gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-common -fno-pic -Wall -Wextra -Werror -I"%ROOT%include" -c "%%f" -o "%%~dpnf.o"
        if !ERRORLEVEL! NEQ 0 exit /b !ERRORLEVEL!
    )
)

:: Find all object files
set OBJ_FILES=
for /r "%ROOT%" %%f in (*.o) do set "OBJ_FILES=!OBJ_FILES! "%%f""

if "%OBJ_FILES%"=="" (
    echo Error: No object files found to link
    exit /b 1
)

:: Link kernel
echo Linking kernel...
if not exist "%ROOT%linker.ld" (
    echo Error: linker.ld not found
    exit /b 1
)

ld -m i386pe -T "%ROOT%linker.ld" -o "%KERNEL_BIN%" %OBJ_FILES%
if %ERRORLEVEL% NEQ 0 (
    echo Error linking kernel
    exit /b 1
)

:: Create disk image
echo Creating disk image...
fsutil file createnew "%KERNEL_IMG%" 1474560 >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: Could not create disk image. Try running as Administrator.
    exit /b 1
)

:: Copy bootloader to image
type "%BOOTLOADER%" > "%KERNEL_IMG%"
if %ERRORLEVEL% NEQ 0 (
    echo Error writing bootloader to image
    exit /b 1
)

:: Append kernel to image
copy /b "%KERNEL_IMG%" + "%KERNEL_BIN%" "%TEMP_KERNEL%" >nul
if %ERRORLEVEL% NEQ 0 (
    echo Error appending kernel to image
    exit /b 1
)
move /y "%TEMP_KERNEL%" "%KERNEL_IMG%" >nul

:: Clean up object files
echo Cleaning up...
del /s /q "%ROOT%*.o" 2>nul

:: Check if QEMU is available
where qemu-system-i386 >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! Starting QEMU...
    qemu-system-i386 -fda "%KERNEL_IMG%"
) else (
    echo.
    echo Build successful! QEMU not found in PATH.
    echo To run the kernel, use: qemu-system-i386 -fda "%KERNEL_IMG%"
)

exit /b 0
