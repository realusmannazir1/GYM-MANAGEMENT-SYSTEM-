@echo off
cls
set "QT_PATH=D:\Qt\6.7.2\mingw_64\bin"
set "MINGW_PATH=D:\Qt\Tools\mingw1310_64\mingw64\bin"
set "PATH=%QT_PATH%;%MINGW_PATH%;%SystemRoot%;%SystemRoot%\System32"
cd /d "d:\Projects\GYM MANAGEMENT SYSTEM"
cmake --build build --target FitCore 2> build\build_err.log
echo BUILD_EXIT_CODE=%ERRORLEVEL% > build\status.txt
timeout /t 3 /nobreak > nul
exit /b %ERRORLEVEL%
