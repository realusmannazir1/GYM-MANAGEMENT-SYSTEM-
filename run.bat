@echo off
title FitCore - Gym Management System
cd /d "%~dp0"

rem Qt 6.7.2 runtime must come first, then the matching GCC 13.1 toolchain.
set "QT_BIN=D:\Qt\6.7.2\mingw_64\bin"
set "MINGW_BIN=D:\Qt\Tools\mingw1310_64\mingw64\bin"
set "QT_PLUGINS=D:\Qt\6.7.2\mingw_64\plugins"
set "BUILD=%~dp0build"

set "PATH=%QT_BIN%;%MINGW_BIN%;%SystemRoot%\System32;%PATH%"
set "QT_PLUGIN_PATH=%QT_PLUGINS%"

echo Checking incremental build status...
cmake --build build --target FitCore
if %ERRORLEVEL% NEQ 0 (
    echo Build FAILED.
    exit /b 1
)

rem Deploy Qt runtime DLLs into the build folder so the exe is self-contained.
echo Deploying Qt DLLs...
copy /Y "%QT_BIN%\Qt6Core.dll"        "%BUILD%\Qt6Core.dll"        >nul
copy /Y "%QT_BIN%\Qt6Gui.dll"         "%BUILD%\Qt6Gui.dll"         >nul
copy /Y "%QT_BIN%\Qt6Widgets.dll"     "%BUILD%\Qt6Widgets.dll"     >nul
copy /Y "%QT_BIN%\Qt6Sql.dll"         "%BUILD%\Qt6Sql.dll"         >nul
copy /Y "%QT_BIN%\Qt6Charts.dll"      "%BUILD%\Qt6Charts.dll"      >nul
copy /Y "%QT_BIN%\Qt6Svg.dll"         "%BUILD%\Qt6Svg.dll"         >nul
copy /Y "%QT_BIN%\Qt6Network.dll"     "%BUILD%\Qt6Network.dll"     >nul
copy /Y "%QT_BIN%\D3Dcompiler_47.dll" "%BUILD%\D3Dcompiler_47.dll" >nul
copy /Y "%QT_BIN%\opengl32sw.dll"     "%BUILD%\opengl32sw.dll"     >nul

rem Deploy the matching GCC 13.1 runtime DLLs (MUST match the Qt MinGW build).
echo Deploying MinGW runtime DLLs...
copy /Y "%MINGW_BIN%\libgcc_s_seh-1.dll"  "%BUILD%\libgcc_s_seh-1.dll"  >nul
copy /Y "%MINGW_BIN%\libstdc++-6.dll"     "%BUILD%\libstdc++-6.dll"     >nul
copy /Y "%MINGW_BIN%\libwinpthread-1.dll" "%BUILD%\libwinpthread-1.dll" >nul

rem Deploy Qt plugins (platforms, sqldrivers, styles, ...).
echo Deploying Qt plugins...
if not exist "%BUILD%\platforms"         mkdir "%BUILD%\platforms"
copy /Y "%QT_PLUGINS%\platforms\qwindows.dll" "%BUILD%\platforms\qwindows.dll" >nul
if not exist "%BUILD%\sqldrivers"        mkdir "%BUILD%\sqldrivers"
copy /Y "%QT_PLUGINS%\sqldrivers\qsqlite.dll" "%BUILD%\sqldrivers\qsqlite.dll" >nul
if not exist "%BUILD%\styles"            mkdir "%BUILD%\styles"
copy /Y "%QT_PLUGINS%\styles\*.dll"      "%BUILD%\styles" >nul
if not exist "%BUILD%\imageformats"      mkdir "%BUILD%\imageformats"
copy /Y "%QT_PLUGINS%\imageformats\*.dll" "%BUILD%\imageformats" >nul
if not exist "%BUILD%\iconengines"       mkdir "%BUILD%\iconengines"
copy /Y "%QT_PLUGINS%\iconengines\*.dll" "%BUILD%\iconengines" >nul
if not exist "%BUILD%\tls"               mkdir "%BUILD%\tls"
copy /Y "%QT_PLUGINS%\tls\*.dll"         "%BUILD%\tls" >nul
if not exist "%BUILD%\networkinformation" mkdir "%BUILD%\networkinformation"
copy /Y "%QT_PLUGINS%\networkinformation\*.dll" "%BUILD%\networkinformation" >nul


echo Code-signing FitCore.exe...
powershell -NoProfile -ExecutionPolicy Bypass -Command "$cert = Get-ChildItem Cert:\CurrentUser\My -CodeSigningCert | Select-Object -First 1; if ($cert) { Set-AuthenticodeSignature -FilePath '%BUILD%\FitCore.exe' -Certificate $cert -HashAlgorithm SHA256 | Out-Null }"

start "" "%BUILD%\FitCore.exe"

