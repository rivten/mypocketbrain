@echo off

set ZigExePath=C:\zig\zig.exe

echo Building Win32...

pushd ..

%ZigExePath% build

popd

echo Compilation completed...
