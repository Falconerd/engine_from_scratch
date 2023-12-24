#include "common.h"
#include "win32.h"
#include "math.c"
#include "util.c"

#include "game.c"

#define ErrorBox(m) MessageBox(0, m, "Error", 0)

////////////////////////////////////////////////////////////////////////////////

void debug_platform_free_file_memory(void *ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
}

result debug_platform_read_entire_file(const char *file_name) {
    result r = {0};
    void *h = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (h != INVALID_HANDLE_VALUE) {
        large_integer file_size;
        if (GetFileSize(h, &file_size)) {
            u32 file_size_32 = safe_truncate_u64(file_size.QuadPart);
            void *data = VirtualAlloc(0, file_size_32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (data) {
                u32 bytes_read;
                if (ReadFile(h, data, (u32)file_size.QuadPart, &bytes_read, 0) && file_size_32 == bytes_read) {
                    r.size = file_size_32;
                    r.data = data;
                } else {
                    debug_platform_free_file_memory(data);
                }
            }
        }
        CloseHandle(h);
    }

    return r;
}

b32 debug_platform_write_entire_file(const char *file_name, u32 size, void *ptr) {
    b32 ok = 0;
    void *h = CreateFile(file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (h != INVALID_HANDLE_VALUE) {
        u32 bytes_written;
        if (WriteFile(h, ptr, size, &bytes_written, 0)) {
            ok = bytes_written == size;
        }
        CloseHandle(h);
    }

    return ok;
}

////////////////////////////////////////////////////////////////////////////////

void *win32_window_proc(void *h, u32 m, void *l, void *w) {
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
            return DefWindowProc(h, m, l, w);
    }
}

#include "df.h"

void *test_alloc(i64 size, void *context) {
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

int __stdcall WinMain(void *instance, void *prev_instance, const char *command_line, int show_code) {
    (void)instance; (void)prev_instance; (void)command_line; (void)show_code;

    w32windowclass wc = {0, win32_window_proc, 0, 0, 0, 0, 0, 0, 0, WINDOW_TITLE};

    if (!RegisterClass(&wc)) {
        MessageBox(0, "Failed to register window class", "Error", 0);
        return 1;
    }

    allocator a = {
        .alloc = test_alloc,
    };

    int *arr = array(int, a);
    array_put(arr, 42);
    array_put(arr, 23);
    array_put(arr, 13);
    array_put(arr, 17);
    array_put(arr, 42);
    array_put(arr, 23);
    array_put(arr, 13);
    array_put(arr, 17);
    array_put(arr, 42);
    array_put(arr, 23);
    array_put(arr, 13);
    array_put(arr, 17);

    s8 s = s8("This is a test string.");

    s8 s2 = s8fromstr((const char *)s.data, a);

    __debugbreak();

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_position_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_position_y = (screen_height - WINDOW_HEIGHT) / 2;

    w32window window = CreateWindow(0, WINDOW_TITLE, WINDOW_TITLE, WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE, window_position_x, window_position_y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0);
    if (!window) {
        MessageBox(0, "Failed to create window", "Error", 0);
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////

    u64 ba = TB(4);
    void *base_address = (void *)ba;

    game_memory gm = {.permanent_storage_size = MB(64), .transient_storage_size = MB(64)};
    u64 size = gm.permanent_storage_size + gm.transient_storage_size;

    gm.permanent_storage = VirtualAlloc(base_address, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    gm.transient_storage = ((u8 *)gm.permanent_storage) + gm.permanent_storage_size;

    ASSERT(gm.permanent_storage && "Failed to allocate memory.");

    gm.is_initialized = 1;

    ////////////////////////////////////////////////////////////////////////////

    input_state input = {0};

    ////////////////////////////////////////////////////////////////////////////

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

