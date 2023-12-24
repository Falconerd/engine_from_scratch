#include "win32.h"

typedef struct {
    i64 size;
    byte *data;
} osfile;

osfile osreadfile(char *name, allocator a) {
    osfile f = {0};
    void *h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (h != INVALID_HANDLE_VALUE) {
        large_integer file_size;
        if (GetFileSize(h, &file_size)) {
            assert(file_size.QuadPart <= MAX_U32);
            u32 file_size_32 = (u32)file_size.QuadPart;
            byte *buffer = make(byte, file_size_32, a);
            if (buffer) {
                u32 bytes_read;
                if (ReadFile(h, buffer, file_size_32, &bytes_read, 0) && file_size_32 == bytes_read) {
                    f.size = file_size_32;
                    f.data = buffer;
                } else {
                    delete(file_size_32, buffer, a);
                }
            }
        }
        CloseHandle(h);
    }
    return f;
}
