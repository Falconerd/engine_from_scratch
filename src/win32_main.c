#include "common.h"
#include "win32.h"
#include "game.c"

#define ErrorBox(m) MessageBox(0, m, "Error", 0)

read_file_result debug_platform_read_entire_file(const char *file_name) {
    read_file_result result = {0};
    HANDLE h = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (h != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (GetFileSize(h, &file_size)) {
            u32 file_size_32 = safe_truncate_u64(file_size.QuadPart);
            void *data = VirtualAlloc(0, file_size_32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (data) {
                DWORD bytes_read;
                if (ReadFile(h, data, file_size.QuadPart, &bytes_read, 0) && file_size_32 == bytes_read) {
                    result.size = file_size_32;
                    result.data = data;
                } else {
                    debug_platform_free_file_memory(data);
                }
            }
        }
        CloseHandle(h);
    }

    return result;
}

void debug_platform_free_file_memory(void *ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
}

b32 debug_platform_write_entire_file(const char *file_name, u64 size, void *ptr) {
    b32 result = 0;
    HANDLE h = CreateFile(file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD bytes_written;
        if (WriteFile(h, ptr, size, &bytes_written, 0)) {
            result = bytes_written == size;
        }
        CloseHandle(h);
    }

    return result;
}

void WinMainCRTStartup(void) {
    HINSTANCE handle = GetModuleHandle(0);
    int r = WinMain(handle, 0, 0, 0);
    ExitProcess(r);
}

int __stdcall WinMain(void *instance, void *prev_instance, const char *command_line, int show_code) {
    WNDCLASSA wc = {0, DefWindowProc, 0, 0, 0, 0, 0, 0, 0, WINDOW_TITLE};

    if (!RegisterClass(&wc)) {
        MessageBox(0, "Failed to register window class", "Error", 0);
        return 1;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_position_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_position_y = (screen_height - WINDOW_HEIGHT) / 2;

    HWND window = CreateWindow(0, WINDOW_TITLE, WINDOW_TITLE, WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               window_position_x, window_position_y, WINDOW_WIDTH, WINDOW_HEIGHT,
                               0, 0, 0, 0);
    if (!window) {
        MessageBox(0, "Failed to create window", "Error", 0);
        return 1;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    u64 ba = TB(4);
    void *base_address = (void *)ba;

    game_memory gm = {.permanent_storage_size = MB(64), .transient_storage_size = MB(64)};
    u64 size = gm.permanent_storage_size + gm.transient_storage_size;

    gm.permanent_storage = VirtualAlloc(base_address, gm.permanent_storage_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    gm.transient_storage = ((u8 *)gm.permanent_storage) + gm.permanent_storage_size;

#ifndef BUILD_SPEED
    ASSERT(gm.permanent_storage && "Failed to allocate memory.");
#endif

    gm.is_initialized = 1;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    b32 is_running = 1;
    while (is_running) {
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_KEYDOWN) {
                if (MessageBox(0, "Really quit?", "", MB_OKCANCEL) == IDOK) {
                    is_running = 0;
                }
                break;
            }
            DispatchMessage(&msg);
        }

        game_update_and_render(&gm);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
