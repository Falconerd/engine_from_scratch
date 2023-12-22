#ifndef DRF_WIN32_H
#define DRF_WIN32_H

#define CONST const
#define CALLBACK __stdcall
typedef int                 BOOL;
typedef long                LONG;
typedef char                CHAR;
typedef void *              PVOID;
typedef void *              LPVOID;
typedef PVOID               HANDLE;
typedef HANDLE              HINSTANCE;
typedef HINSTANCE           HMODULE;
typedef CONST CHAR *        LPCSTR;
typedef unsigned int        UINT;
typedef HANDLE              HWND;
typedef LPCSTR              LPCTSTR;
typedef unsigned short      WORD;
typedef WORD                ATOM;
typedef unsigned long       DWORD;
typedef HANDLE              HMENU;
typedef __int64             LONG_PTR;
typedef LONG_PTR            LRESULT;
typedef unsigned __int64    UINT_PTR;
typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef HANDLE              HICON;
typedef HICON               HCURSOR;
typedef HICON               HBRUSH;

typedef struct tagPOINT {
    LONG x;
    LONG y;
} POINT;

typedef struct tagMSG {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
} MSG;

typedef MSG *               LPMSG;

#define CW_USEDEFAULT       ((int)0x80000000)

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define WS_POPUPWINDOW      (WS_POPUP | WS_BORDER | WS_SYSMENU)
#define WS_CHILDWINDOW      (WS_CHILD)

#define WM_NULL             0x0000
#define WM_CREATE           0x0001
#define WM_DESTROY          0x0002
#define WM_MOVE             0x0003
#define WM_SIZE             0x0005
#define WM_ACTIVATE         0x0006

#define WA_INACTIVE         0
#define WA_ACTIVE           1
#define WA_CLICKACTIVE      2

#define PM_REMOVE           0x0001

#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101
#define WM_CHAR             0x0102
#define WM_DEADCHAR         0x0103
#define WM_SYSKEYDOWN       0x0104
#define WM_SYSKEYUP         0x0105
#define WM_SYSCHAR          0x0106
#define WM_SYSDEADCHAR      0x0107
#define WM_UNICHAR          0x0109
#define WM_KEYLAST          0x0109

#define SM_CXSCREEN         0
#define SM_CYSCREEN         1

#define IDOK                1
#define IDCANCEL            2

#define MB_OK               0x00000000L
#define MB_OKCANCEL         0x00000001L

typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct WNDCLASS {
  UINT style;
  WNDPROC lpfnWndProc;
  int cbClsExtra;
  int cbWndExtra;
  HINSTANCE hInstance;
  HICON hIcon;
  HCURSOR hCursor;
  HBRUSH hbrBackground;
  LPCSTR lpszMenuMane;
  LPCSTR lpszClassName;
} WNDCLASSA, WNDCLASS;

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPCSTR, int);

#define GetModuleHandle GetModuleHandleA
HMODULE __stdcall GetModuleHandleA(LPCSTR);

#define CreateWindow CreateWindowExA
HWND __stdcall CreateWindowExA(DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);

#define ShowWindow ShowWindow
BOOL ShowWindow(HWND, int);

BOOL UpdateWindow(HWND);

#define DefWindowProc DefWindowProcA
LRESULT DefWindowProcA(HWND, UINT, WPARAM, LPARAM);

#define RegisterClass RegisterClassA
ATOM RegisterClassA(const WNDCLASS *);

#define GetMessag GetMessageA
BOOL GetMessageA(LPMSG, HWND, UINT, UINT);

BOOL TranslateMessage(const MSG *);

#define DispatchMessage DispatchMessageA
LRESULT DispatchMessageA(const MSG *);

#define PeekMessage PeekMessageA
BOOL PeekMessageA(LPMSG, HWND, UINT, UINT, UINT);

#define MessageBox MessageBoxA
int __stdcall MessageBoxA(HWND, LPCSTR, LPCSTR, UINT);

int GetSystemMetrics(int);

#endif