@echo off
title FitCore — Gym Management System
cd /d "%~dp0"

set PATH=D:\Software\mingw64\mingw64\bin;D:\Qt\6.7.2\mingw_64\bin;%PATH%
set QT_PLUGIN_PATH=D:\Qt\6.7.2\mingw_64\plugins

echo Checking incremental build status...
cmake --build build --target FitCore
if %ERRORLEVEL% EQU 0 (
    rem Deploy Qt6Charts.dll if missing
    if not exist "%~dp0build\Qt6Charts.dll" (
        echo Deploying Qt6Charts.dll...
        copy /Y "D:\Qt\6.7.2\mingw_64\bin\Qt6Charts.dll" "%~dp0build\Qt6Charts.dll"
    )
    powershell -NoProfile -ExecutionPolicy Bypass -Command "$cert = Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Select-Object -First 1; if ($cert) { Set-AuthenticodeSignature -FilePath '%~dp0build\FitCore.exe' -Certificate $cert | Out-Null }"
    echo Launching FitCore...
    start "" "%~dp0build\FitCore.exe"
)
