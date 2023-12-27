#include "common.h"
#include "mem.c"
#include "os.c"
#include "array.c"
#include "math.c"
#include "s8.c"
#include "arena.c"
#include "draw.c"
#include "game.c"

#define ErrorBox(m) MessageBox(0, m, "Error", 0)

////////////////////////////////////////////////////////////////////////////////////////////////////

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

void *proc_load(char *name) {
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        void *module = LoadLibrary("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }
    return p;
}

#define proc_load_assign(n) n = (n##def *)proc_load(#n); assert(n && "Failed to load n");

int __stdcall WinMain(void *inst, void *previnst, const char *cline, int showcode) {
    (void)inst; (void)previnst; (void)cline; (void)showcode;

    WNDCLASSA wc = {0, window_proc, 0, 0, 0, 0, 0, 0, 0, WINDOW_TITLE};

    if (!RegisterClass(&wc)) {
        MessageBox(0, "Failed to register window class", "Error", 0);
        return 1;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_position_x = (screen_width - WINDOW_WIDTH) / 2;
    int window_position_y = (screen_height - WINDOW_HEIGHT) / 2;

    HWND window = CreateWindow(0, WINDOW_TITLE, WINDOW_TITLE,
                               WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               window_position_x, window_position_y, WINDOW_WIDTH, WINDOW_HEIGHT,
                               0, 0, 0, 0);
    if (!window) {
        MessageBox(0, "Failed to create window", "Error", 0);
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Allocate all the memory for the game.
    u64 ba = TB(4);
    void *base_addr = (void *)ba;

    game_memory gm = {.permanent_storage_size = MB(64), .transient_storage_size = MB(64)};

    u64 size = gm.permanent_storage_size + gm.transient_storage_size;

    gm.permanent_storage = VirtualAlloc(base_addr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    gm.transient_storage = ((u8 *)gm.permanent_storage) + gm.permanent_storage_size;

    assert(gm.permanent_storage && "Failed to allocate memory.");

    gm.is_initialized = 1;

    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Create OpenGL Context.
    HDC dc = GetDC(window);

    PIXELFORMATDESCRIPTOR desired_pixel_format = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
        .cColorBits = 24,
        .cAlphaBits = 8,
    };

    int chosen_index = ChoosePixelFormat(dc, &desired_pixel_format);
    PIXELFORMATDESCRIPTOR chosen_pixel_format = {0};
    DescribePixelFormat(dc, chosen_index, sizeof(PIXELFORMATDESCRIPTOR), &chosen_pixel_format);
    SetPixelFormat(dc, chosen_index, &chosen_pixel_format);

    HGLRC glc;
    HGLRC temp_glc = wglCreateContext(dc);
    assert(wglMakeCurrent(dc, temp_glc) && "Failed to make OpenGL context current.");

    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Get the proc address of all the required functions first.
    // If this fails, there's no point continuing.
    proc_load_assign(wglGetExtensionsStringARB);
    proc_load_assign(wglChoosePixelFormatARB);
    proc_load_assign(wglCreateContextAttribsARB);

    proc_load_assign(glCreateShader);
    proc_load_assign(glCompileShader);
    proc_load_assign(glShaderSource);
    proc_load_assign(glGetShaderiv);
    proc_load_assign(glGetShaderInfoLog);
    proc_load_assign(glCreateProgram);
    proc_load_assign(glAttachShader);
    proc_load_assign(glLinkProgram);
    proc_load_assign(glGetProgramInfoLog);
    proc_load_assign(glGetProgramiv);
    proc_load_assign(glGenVertexArrays);
    proc_load_assign(glBindVertexArray);
    proc_load_assign(glGenBuffers);
    proc_load_assign(glBindBuffer);
    proc_load_assign(glBufferData);
    proc_load_assign(glVertexAttribPointer);
    proc_load_assign(glEnableVertexAttribArray);
    proc_load_assign(glBindVertexArray);
    proc_load_assign(glDrawArrays);
    proc_load_assign(glGetError);
    proc_load_assign(glUseProgram);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Ensure that the required extensions are available.
    s8 wgl_extensions = s8((char *)wglGetExtensionsStringARB(dc));

    b32 has_float_pixels = s8_contains(wgl_extensions, s8("WGL_ARB_pixel_format_float"));
    assert(has_float_pixels && "Could not find required extension.");

    b32 has_framebuffer_sRGB = s8_contains(wgl_extensions, s8("WGL_EXT_framebuffer_sRGB"));
        has_framebuffer_sRGB |= s8_contains(wgl_extensions, s8("WGL_ARB_framebuffer_sRGB"));
    assert(has_framebuffer_sRGB && "Could not find required extension.");
    assert(s8_contains(wgl_extensions, s8("WGL_ARB_multisample")) && "Could not find required extension.");

    int pflist[] = {
        WGL_DRAW_TO_WINDOW_ARB, 1,
        WGL_SUPPORT_OPENGL_ARB, 1,
        WGL_DOUBLE_BUFFER_ARB, 1,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0,
    };

    int clist[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        0,
    };

    int pf;
    u32 n;

    b32 success = wglChoosePixelFormatARB(dc, pflist, 0, 1, &pf, &n);
    assert(success && "Failed to get pixel format.");
    glc = wglCreateContextAttribsARB(dc, temp_glc, clist);
    assert(glc && "Failed to create OpenGL context.");
    assert(wglMakeCurrent(dc, glc) && "Failed to make OpenGL context current.");
    wglDeleteContext(temp_glc);

    // TODO: Check for extensions used by the game.

    ////////////////////////////////////////////////////////////////////////////////////////////////

    input_state input = {0};

    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    game_init(&gm);

    ////////////////////////////////////////////////////////////////////////////////////////////////

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

        glClearColor(0.f, 0.105f, 0.105f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_update_and_render(&input);

        SwapBuffers(dc);
    }
}

void WinMainCRTStartup(void) {
    void *handle = GetModuleHandle(0);
    int r = WinMain(handle, 0, 0, 0);
    ExitProcess(r);
}

