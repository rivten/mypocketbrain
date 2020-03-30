@echo off

echo Building Win32...

pushd ..

%ZigExePath% build

popd

echo Compilation completed...
