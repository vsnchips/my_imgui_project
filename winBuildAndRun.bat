@echo off
setlocal

REM Set the path to your vcvars64.bat file
set VCVARS_BAT="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM Set the build directory
set BUILD_DIR=./build

REM Run vcvars64.bat to set up the environment
call %VCVARS_BAT%

REM Change to the build directory
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

REM Generate Ninja build files using CMake
cmake -G "Ninja" ..

REM Build your project using Ninja
ninja

endlocal
