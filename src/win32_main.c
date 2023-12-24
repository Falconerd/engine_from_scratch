#include "common.h"
#include "mem.c"
#include "os.c"
#include "array.c"
#include "math.c"
#include "s8.c"

#include "game.c"

#define ErrorBox(m) MessageBox(0, m, "Error", 0)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

i64 win32_window_proc(void *h, u32 m, u64 w, i64 l) {
    switch (m) {
        case WM_CLOSE: {
            DestroyWindow(h);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProc(h, m, w, l);
    }
}

void *test_alloc(i64 size, void *context) {
    (void)context;
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

int __stdcall WinMain(void *instance, void *prev_instance, const char *command_line, int show_code) {
    (void)instance; (void)prev_instance; (void)command_line; (void)show_code;

    w32windowclass wc = {0, win32_window_proc, 0, 0, 0, 0, 0, 0, 0, WINDOW_TITLE};

    if (!RegisterClass(&wc)) {
        MessageBox(0, "Failed to register window class", "Error", 0);
        return 1;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_position_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_position_y = (screen_height - WINDOW_HEIGHT) / 2;

    w32window window = CreateWindow(0, WINDOW_TITLE, WINDOW_TITLE, WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                    window_position_x, window_position_y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0);
    if (!window) {
        MessageBox(0, "Failed to create window", "Error", 0);
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    u64 ba = TB(4);
    void *base_address = (void *)ba;

    game_memory gm = {.permanent_storage_size = MB(64), .transient_storage_size = MB(64)};
    u64 size = gm.permanent_storage_size + gm.transient_storage_size;

    gm.permanent_storage = VirtualAlloc(base_address, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    gm.transient_storage = ((u8 *)gm.permanent_storage) + gm.permanent_storage_size;

    assert(gm.permanent_storage && "Failed to allocate memory.");

    gm.is_initialized = 1;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    input_state input = {0};

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (;;) {
        w32msg msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return 0;
            }

            if (msg.message == WM_KEYDOWN) {
                if (msg.wParam == VK_ESCAPE) {
                    if (MessageBox(0, "Really quit?", "Aww", MB_OKCANCEL) == IDOK) {
                        return 0;
                    }
                }

                // TODO: Key mapping.
                if (msg.wParam == 'F') input.forward.down = 1;
                if (msg.wParam == 'S') input.backward.down = 1;
                if (msg.wParam == 'R') input.left.down = 1;
                if (msg.wParam == 'T') input.right.down = 1;
                break;
            }

            if (msg.message == WM_KEYUP) {
                if (msg.wParam == 'F') input.forward.down = 0;
                if (msg.wParam == 'S') input.backward.down = 0;
                if (msg.wParam == 'R') input.left.down = 0;
                if (msg.wParam == 'T') input.right.down = 0;
                break;
            }

            DispatchMessage(&msg);
        }

        game_update_and_render(&gm, &input);
    }
}

void WinMainCRTStartup(void) {
    void *handle = GetModuleHandle(0);
    int r = WinMain(handle, 0, 0, 0);
    ExitProcess(r);
}

