#ifndef ___DUCKED_C_WINDOWS_HPP___
#define ___DUCKED_C_WINDOWS_HPP___

#include <fcntl.h>
#include <io.h>
#include <windows.h>

void __DUCKLIB_WINDOWS_INIT__() {
    _setmode(0, _O_BINARY);
    _setmode(1, _O_BINARY);
    SetConsoleCP(CP_UTF8);  // maybe will work someday
    SetConsoleOutputCP(CP_UTF8);
}

#endif