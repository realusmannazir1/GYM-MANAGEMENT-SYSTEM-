@echo off
title FitCore — Gym Management System
cd /d "%~dp0"

set PATH=D:\Qt\6.7.2\mingw_64\bin;%PATH%
set QT_PLUGIN_PATH=D:\Qt\6.7.2\mingw_64\plugins

echo Checking incremental build status...
cmake --build build --target FitCore
if %ERRORLEVEL% EQU 0 (
    echo Launching FitCore...
    start "" "build\FitCore.exe"
) else (
    echo Build failed. Please inspect errors above.
    pause
)
