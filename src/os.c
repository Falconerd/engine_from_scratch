#ifndef OS_INCLUDED
#define OS_INCLUDED

#include "win32.h"

typedef struct {
    i64 size;
    byte *data;
} os_file;

os_file os_file_read(char *name, allocator a) {
    os_file f = {0};
    void *h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (h != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER fs;
        if (GetFileSize(h, &fs)) {
            assert(fs.QuadPart <= MAX_U32);
            u32 fs_32 = (u32)fs.QuadPart;
            byte *buffer = make(byte, fs_32, a);
            if (buffer) {
                u32 bytes_read;
                if (ReadFile(h, buffer, fs_32, &bytes_read, 0) && fs_32 == bytes_read) {
                    f.size = fs_32;
                    f.data = buffer;
                } else {
                    delete(fs_32, buffer, a);
                }
            }
        }
        CloseHandle(h);
    }
    return f;
}

#endif

