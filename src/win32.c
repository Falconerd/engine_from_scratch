#include "win32.h"

void __stdcall WinMainCRTStartup(void) {
    HINSTANCE handle = GetModuleHandle(0);
    // __debugbreak();
    int r = WinMain(handle, 0, 0, 0);
    ExitProcess(r);
}
