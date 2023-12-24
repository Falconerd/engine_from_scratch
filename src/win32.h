#ifndef WIN32_H
#define WIN32_H

#include "common.h"

typedef void * (__stdcall *w32windowproc)(void *, u32, void *, void *);
typedef void * w32window;

typedef union _LARGE_INTEGER {
  struct {
    u32 LowPart;
    i32 HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    u32 LowPart;
    i32 HighPart;
  } u;
  i64 QuadPart;
} large_integer;

typedef struct {
    i32 x;
    i32 y;
} w32point;

typedef struct {
    void *hwnd;
    u32 message;
    i32 wParam;
    void *lParam;
    u32 time;
    w32point pt;
} w32msg;

#define W32(r) __declspec(dllimport) r __stdcall
#define CreateFile CreateFileA
W32(byte *) CreateFileA(const char *, u32, u32, void *, u32, u32, void *);
W32(byte *) VirtualAlloc(void *, size, u32, u32);
#define DefWindowProc DefWindowProcA
W32(void *) DefWindowProcA(void *, u32, void *, void *);
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
W32(b32) PeekMessageA(w32msg *, void *, u32, u32, u32);
#define DispatchMessage DispatchMessageA
W32(void *) DispatchMessageA(w32msg *);
#define GetFileSize GetFileSizeEx
W32(i32) GetFileSizeEx(void *, large_integer *);
W32(b32) ReadFile(void *, void *, u32, u32 *, void *);
W32(b32) CloseHandle(void *);
W32(b32) DestroyWindow(void *);
W32(b32) PostQuitMessage(void *);
W32(void) ExitProcess(u32);
int GetSystemMetrics(int);

////////////////////////////////////////////////////////////////////////////////

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_VISIBLE          0x10000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
                             WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

#define WM_DESTROY          0x0002
#define WM_CLOSE            0x0010
#define WM_QUIT             0x0012

#define PM_REMOVE           0x0001

#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101

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

#define VK_ESCAPE             0x1B

////////////////////////////////////////////////////////////////////////////////

typedef struct {
  u32 style;
  w32windowproc lpfnWndProc;
  int cbClsExtra;
  int cbWndExtra;
  void *hInstance;
  void *hIcon;
  void *hCursor;
  void *hbrBackground;
  char *lpszMenuMane;
  char *lpszClassName;
} w32windowclass;

#endif