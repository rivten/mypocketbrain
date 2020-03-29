@echo off

set ZigExePath=C:\zig\zig.exe
set RaylibPath=C:\raylib\lib\raylib.lib
set RaylibInclude=C:\raylib\include\

set UntreatedWarnings=/wd4100 /wd4244 /wd4201 /wd4127 /wd4505 /wd4456 /wd4996 /wd4003 /wd4706 /wd4200 /wd4189
set CommonCompilerDebugFlags=/MT /Od /Oi /fp:fast /fp:except- /Zo /Gm- /GR- /EHa /WX /W4 %UntreatedWarnings% /Z7 /nologo /DCOMPILE_SLOW=1 /DCOMPILE_INTERNAL=1 /DCOMPILE_WINDOWS=1 /I %RaylibInclude%
set CommonLinkerDebugFlags=/incremental:no /opt:ref /subsystem:windows /ignore:4099 /NODEFAULTLIB


pushd ..\build\

%ZigExePath% cc %CommonCompilerDebugFlags% ..\code\brain.c /link %CommonLinkerDebugFlags% %RaylibPath%
popd

rem --------------------------------------------------------------------------
echo Compilation completed...
