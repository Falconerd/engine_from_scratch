#pragma once
#include "common.h"

typedef i64 (__stdcall *WindowProc)(void *, u32, u64, i64);
typedef void * HWND;
typedef void * HDC;

typedef union {
    struct {
        u32 LowPart;
        i32 HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        u32 LowPart;
        i32 HighPart;
    } u;
    i64 QuadPart;
} LARGE_INTEGER;

typedef struct {
    i32 x;
    i32 y;
} POINT;

typedef struct {
    void *hwnd;
    u32 message;
    u64 wParam;
    i64 lParam;
    u32 time;
    POINT pt;
    u32 lPrivate;
} MSG;

typedef struct {
    u32 style;
    WindowProc lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    void *hInstance;
    void *hIcon;
    void *hCursor;
    void *hbrBackground;
    char *lpszMenuMane;
    char *lpszClassName;
} WNDCLASSA;

typedef struct {
    u16 nSize;
    u16 nVersion;
    u32 dwFlags;
    byte iPixelType;
    byte cColorBits;
    byte cRedBits;
    byte cRedShift;
    byte cGreenBits;
    byte cGreenShift;
    byte cBlueBits;
    byte cBlueShift;
    byte cAlphaBits;
    byte cAlphaShift;
    byte cAccumBits;
    byte cAccumRedBits;
    byte cAccumGreenBits;
    byte cAccumBlueBits;
    byte cAccumAlphaBits;
    byte cDepthBits;
    byte cStencilBits;
    byte cAuxBuffers;
    byte iLayerType;
    byte bReserved;
    u32 dwLayerMask;
    u32 dwVisibleMask;
    u32 dwDamageMask;
} PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_VISIBLE          0x10000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

#define WM_DESTROY          0x0002
#define WM_CLOSE            0x0010
#define WM_QUIT             0x0012
#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101
#define WM_MOUSEMOVE        0x0200
#define LOWORD(l)           ((u16)(((uptr)(l)) & 0xffff))
#define GET_X_LPARAM(lp)    ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)    ((int)(short)HIWORD(lp))
#define HIWORD(l)           ((u16)((((uptr)(l)) >> 16) & 0xffff))

#define PM_REMOVE           0x0001

#define SM_CXSCREEN         0
#define SM_CYSCREEN         1

#define IDOK                1

#define MB_OKCANCEL         0x00000001L

#define MEM_COMMIT          0x00001000
#define MEM_RESERVE         0x00002000
#define MEM_RELEASE         0x00008000

#define PAGE_READWRITE      0x04

#define FILE_SHARE_READ     0x00000001

#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3

#define GENERIC_READ        (0x80000000L)
#define GENERIC_WRITE       (0x40000000L)

#define INVALID_HANDLE_VALUE (void *)-1

#define VK_ESCAPE           0x1B

#define PFD_SUPPORT_OPENGL  0x00000020
#define PFD_DRAW_TO_WINDOW  0x00000004
#define PFD_DOUBLEBUFFER    0x00000001

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define W32(r) __declspec(dllimport) r __stdcall
#define CreateFile CreateFileA
W32(byte *) CreateFileA(const char *, u32, u32, void *, u32, u32, void *);
W32(byte *) VirtualAlloc(void *, size, u32, u32);
#define DefWindowProc DefWindowProcA
W32(i64) DefWindowProcA(void *, u32, u64, i64);
#define CreateWindow CreateWindowExA
W32(void *) CreateWindowExA(u32, void *, void *, u32, i32, i32, i32, i32,
                            void *, void *, void *, void *);
#define GetModuleHandle GetModuleHandleA
W32(void *) GetModuleHandleA(void *);
#define MessageBox MessageBoxA
W32(i32) MessageBoxA(void *, void *, void *, u32);
#define RegisterClass RegisterClassA
W32(u16) RegisterClassA(void *);
#define PeekMessage PeekMessageA
W32(b32) PeekMessageA(MSG *, HWND, u32, u32, u32);
#define DispatchMessage DispatchMessageA
W32(i64) DispatchMessageA(MSG *);
#define GetFileSize GetFileSizeEx
W32(i32) GetFileSizeEx(void *, LARGE_INTEGER *);
W32(b32) ReadFile(void *, void *, u32, u32 *, void *);
W32(b32) CloseHandle(void *);
W32(b32) DestroyWindow(void *);
W32(b32) PostQuitMessage(void *);
W32(void) ExitProcess(u32);
int GetSystemMetrics(int);
W32(void *) GetDC(HWND);
W32(int) ChoosePixelFormat(HDC, PIXELFORMATDESCRIPTOR *);
W32(void) DescribePixelFormat(HDC, int, int, PIXELFORMATDESCRIPTOR *);
W32(u32) GetLastError();
W32(b32) SetPixelFormat(HDC, int, const PIXELFORMATDESCRIPTOR *);
W32(b32) SwapBuffers(HDC);
#define LoadLibrary LoadLibraryA
W32(void *) LoadLibraryA(char *);
W32(b32) FreeLibrary(void *);
W32(void *) GetProcAddress(void *, char *);
#define CopyFile CopyFileA
W32(b32) CopyFileA(const char *, const char *, b32);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_DRAW_TO_WINDOW_ARB        0x2001
#define WGL_SUPPORT_OPENGL_ARB        0x2010
#define WGL_DOUBLE_BUFFER_ARB         0x2011
#define WGL_PIXEL_TYPE_ARB            0x2013
#define WGL_TYPE_RGBA_ARB             0x202B
#define WGL_COLOR_BITS_ARB            0x2014
#define WGL_DEPTH_BITS_ARB            0x2022
#define WGL_STENCIL_BITS_ARB          0x2023

typedef void * HGLRC;
void *wglCreateContext(HDC);
void wglDeleteContext(HGLRC);
b32 wglMakeCurrent(HDC, HGLRC);
void *wglGetProcAddress(char *);

// Dynamically loaded via wglGetProcAddress.

TDFP(const char *, wglGetExtensionsStringARB, (HDC));
TDFP(b32, wglChoosePixelFormatARB, (HDC, int *, f32 *, u32, int *, u32 *));
TDFP(HGLRC, wglCreateContextAttribsARB, (HDC, HGLRC, int *));
