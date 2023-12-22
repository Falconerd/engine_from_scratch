#include "common.h"

#define ErrorBox(m) MessageBox(0, m, "Error", 0)

#define TITLE "Test"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static struct state {
    b32 running;
} state = {
    .running = 1,
};

int __stdcall WinMain(void *instance, void *prev_instance, const char *command_line, int show_code) {
    WNDCLASSA wc = {0, DefWindowProc, 0, 0, 0, 0, 0, 0, 0, TITLE};

    if (!RegisterClass(&wc)) {
        MessageBox(0, "Failed to register window class", "Error", 0);
        return 1;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_position_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_position_y = (screen_height - WINDOW_HEIGHT) / 2;

    HWND window = CreateWindow(0, TITLE, TITLE, WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               window_position_x, window_position_y, WINDOW_WIDTH, WINDOW_HEIGHT,
                               0, 0, 0, 0);
    if (!window) {
        MessageBox(0, "Failed to create window", "Error", 0);
        return 1;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    while (state.running) {
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_KEYDOWN) {
                if (MessageBox(0, "Really quit?", "", MB_OKCANCEL) == IDOK) {
                    state.running = 0;
                }
                break;
            }
            DispatchMessage(&msg);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
