#include "win32.h"
#include "win32_main.c"

#endif

void game_update_and_render();

int main(int argc, char *argv[]) {

#ifdef _WIN32

    HINSTANCE handle = GetModuleHandle(0);
    int r = WinMain(handle, 0, 0, 0);
    ExitProcess(r);

#endif

    return 0;
}
