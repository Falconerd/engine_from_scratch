#define CONST const
typedef char CHAR;
typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;
typedef CONST CHAR *LPCSTR;

HMODULE (__stdcall *GetModuleHandle)(LPCSTR lpModuleName);
