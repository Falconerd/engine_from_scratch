#include "win32.h"

int __stdcall WinMain(void *instance, void *prev_instance, char *command_line, int show_code) {
    return(0);
}

void __stdcall WinMainCRTStartup(void) {
    int r = WinMain(GetModuleHandle(0), 0, 0, 0);
    ExitProcess(r);
}
