#include "common.h"
#include "mem.c"
#include "os.c"
#include "array.c"
#include "math.c"
#include "s8.c"

#include "game.c"

#define ErrorBox(m) MessageBox(0, m, "Error", 0)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

i64 window_proc(void *h, u32 m, u64 w, i64 l) {
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

    WNDCLASSA wc = {0, window_proc, 0, 0, 0, 0, 0, 0, 0, WINDOW_TITLE};

    if (!RegisterClass(&wc)) {
        MessageBox(0, "Failed to register window class", "Error", 0);
        return 1;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_position_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_position_y = (screen_height - WINDOW_HEIGHT) / 2;

    HWND window = CreateWindow(0, WINDOW_TITLE, WINDOW_TITLE, WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

    HDC dc = GetDC(window);

    PIXELFORMATDESCRIPTOR desired_pixel_format = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
        .cColorBits = 24,
        .cAlphaBits = 8,
    };

    int suggested_pixel_format_index = ChoosePixelFormat(dc, &desired_pixel_format);
    PIXELFORMATDESCRIPTOR suggested_pixel_format = {0};
    DescribePixelFormat(dc, suggested_pixel_format_index, sizeof(PIXELFORMATDESCRIPTOR), &suggested_pixel_format);
    SetPixelFormat(dc, suggested_pixel_format_index, &suggested_pixel_format);

    HGLRC glc = wglCreateContext(dc);
    if (wglMakeCurrent(dc, glc)) {
        // Get the proc address of all th required functoins first.
        // If this fails, there's no point continuing.
        wglGetExtensionsStringARB = (wglGetExtensionsStringARBdef *)wglGetProcAddress("wglGetExtensionsStringARB");
        assert(wglGetExtensionsStringARB && "Could not load the wglGetExtensionsARB function.");
        wglChoosePixelFormatARB = (wglChoosePixelFormatARBdef *)wglGetProcAddress("wglChoosePixelFormatARB");
        assert(wglChoosePixelFormatARB && "Could not load the wglChoosePixelFormatARB function.");

        // Ensure that the required extensions are available.
        char *wgl_extensions = (char *)wglGetExtensionsStringARB(dc);
        MessageBox(0, wgl_extensions, "Aww", 0);
        __debugbreak();
    } else {
        __debugbreak();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    input_state input = {0};

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (;;) {
        MSG msg;
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

