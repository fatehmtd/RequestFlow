@echo off
REM Build script for RequestFlow Qt project

REM Setup Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

REM Setup Qt environment
set QTDIR=D:\Qt\6.10.0\msvc2022_64
set PATH=%QTDIR%\bin;D:\Qt\Tools\QtCreator\bin\jom;%PATH%

REM Create build directory
if not exist "build" mkdir build
cd build

REM Generate makefiles with output to build directory
qmake ..\RequestFlow.pro -r "CONFIG+=release"

REM Build with jom (parallel make)
jom

REM Deploy Qt dependencies
echo.
echo Deploying Qt dependencies...
%QTDIR%\bin\windeployqt.exe --release RequestFlow\release\RequestFlow.exe

REM Copy project DLLs
copy CoreModel\release\CoreModel.dll RequestFlow\release\ >nul 2>&1
copy CoreView\release\CoreView.dll RequestFlow\release\ >nul 2>&1
copy ExecutionEngine\release\ExecutionEngine.dll RequestFlow\release\ >nul 2>&1

cd ..

echo.
echo Build complete! Artifacts are in the 'build' folder.
echo Run: build\RequestFlow\release\RequestFlow.exe
