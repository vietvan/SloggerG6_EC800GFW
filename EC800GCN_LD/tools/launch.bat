:: Copyright (C) 2018 RDA Technologies Limited and\or its affiliates("RDA").
:: All rights reserved.
::
:: This software is supplied "AS IS" without any warranties.
:: RDA assumes no responsibility or liability for the use of the software,
:: conveys no license or title under any patent, copyright, or mask work
:: right to the product. RDA reserves the right to make changes in the
:: software without notification.  RDA also make no representation or
:: warranty that such application will be suitable for the specified use
:: without further testing or modification.

@echo off
set PROJECT_ROOT=%CD%
set BUILD_TARGET=%1
set BUILD_RELEASE_TYPE=%2
set KCONFIG_CONFIG=%PROJECT_ROOT%\target\%BUILD_TARGET%\target.config
set PROJECT_OUT=%PROJECT_ROOT%\out\%BUILD_TARGET%_%BUILD_RELEASE_TYPE%

set COMPILER_DEFAULT=gcc-arm-none-eabi-9.2.1
for %%I in ("8850") do (
    echo %BUILD_TARGET% | findstr "%%I" > nul && set COMPILER_DEFAULT=gcc-arm-none-eabi-10.2.1)
if "%3"=="" (set COMPILER_VERSION=%COMPILER_DEFAULT%) else (set COMPILER_VERSION=%3)

call :add_path %PROJECT_ROOT%\prebuilts\win32\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\cmake\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\python3
call :add_path %PROJECT_ROOT%\prebuilts\win32\gcc-arm-none-eabi\%COMPILER_VERSION%\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\nanopb
call :add_path %PROJECT_ROOT%\tools
call :add_path %PROJECT_ROOT%\tools\win32

if not exist %PROJECT_OUT% mkdir %PROJECT_OUT%

exit /B 0

:add_path
(echo ";%PATH%;" | findstr /N /I ";%1;" > nul) || set "PATH=%1;%PATH%"
goto :eof
