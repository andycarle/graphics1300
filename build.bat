@echo off
rem Builds and runs a Graphics1300 program on Windows.
rem   Usage:  build.bat             builds main.cpp
rem           build.bat demo.cpp    builds demo.cpp
rem           build.bat swarm       builds every .cpp file in the swarm folder

setlocal EnableDelayedExpansion
set "SRC=%~1"
if "%SRC%"=="" set "SRC=main.cpp"
for %%F in ("%SRC%") do set "NAME=%%~nF"

rem tigr.c only has to be compiled once; after that, builds are fast.
if exist tigr-windows.o goto build
echo Compiling tigr.c - only needed the first time...
g++ -x c -c tigr.c -o tigr-windows.o
if errorlevel 1 goto failed

:build
rem The Graphics1300 library: every .cpp file in the graphics folder.
set "FILES="
for %%F in (graphics\*.cpp) do set "FILES=!FILES! "%%F""

rem Your program: one .cpp file, or every .cpp file in a folder.
if exist "%SRC%\" (
    for %%F in ("%SRC%\*.cpp") do set "FILES=!FILES! "%%F""
    set "RUNDIR=%SRC%"
) else (
    set "FILES=!FILES! "%SRC%""
    set "RUNDIR=."
)

g++ -std=c++17 -I. !FILES! tigr-windows.o -o "!RUNDIR!\%NAME%.exe" -static -lopengl32 -lgdi32
if errorlevel 1 goto failed

echo Built !RUNDIR!\%NAME%.exe - running it...
pushd "!RUNDIR!"
"%NAME%.exe"
popd
exit /b 0

:failed
echo.
echo Build FAILED - see the errors above.
exit /b 1
