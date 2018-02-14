@Echo off
SETLOCAL EnableDelayedExpansion

md build_win32
cd build_win32
cmake -G "Visual Studio 14 2015" ..
cd ..
cmake --build build_win32 --config Debug
cmake --build build_win32 --config Release

md build_win64
cd build_win64
cmake -G "Visual Studio 14 2015 Win64" ..
cd ..
cmake --build build_win64 --config Debug
cmake --build build_win64 --config Release
