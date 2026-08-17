@echo off
title FitCore — Gym Management System
cd /d "%~dp0"

echo Launching FitCore Gym Management System...
if exist "build\FitCore.exe" (
    start "" "build\FitCore.exe"
) else (
    echo Error: build\FitCore.exe not found.
    pause
)
