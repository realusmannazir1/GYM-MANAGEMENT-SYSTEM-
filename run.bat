@echo off
title FitCore — Gym Management System
cd /d "%~dp0"

set PATH=D:\Qt\6.7.2\mingw_64\bin;%PATH%
set QT_PLUGIN_PATH=D:\Qt\6.7.2\mingw_64\plugins

echo Launching FitCore Gym Management System...
if exist "build\FitCore.exe" (
    start "" "build\FitCore.exe"
) else (
    echo Error: build\FitCore.exe not found.
    pause
)
