@echo off
set "QT_BIN=D:\Qt\6.7.2\mingw_64\bin"
set "MINGW_BIN=D:\Qt\Tools\mingw1310_64\mingw64\bin"
set "PATH=%QT_BIN%;%MINGW_BIN%;%SystemRoot%\System32;%PATH%"
cd /d "d:\Projects\GYM MANAGEMENT SYSTEM"
cmake --build build --target FitCore > build\build_out.log 2>&1
echo BUILD_DONE > build\build_status.txt
