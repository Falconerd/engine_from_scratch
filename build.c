#include <windows.h>
#include <stdio.h>
#include <string.h>
/*
    -nologo: Disable the M$/Windows logo crap.
    -GR-: Disable run-time type information.
    -GS-: Disable stack checking.
    -Gs1000000000: Turn off stack probes.
    -Oi: Enable compiler intrinsics.
*/
#define CFLAGS "-nologo -GR- -GS- -Gs1000000000 -Oi"
#define CC "cl"

/*
    -nodefaultlib: Don't link CRT or auto-link kernel32 etc.
    -subsytem:windows: Create a windows application (not console).
    -stack...: Commit a larger stack.
*/
#define LINK_ARGS "-nodefaultlib -subsystem:windows -stack:0x200000,200000"
#define LIBS "kernel32.lib user32.lib"

int main(int argc, char *argv[]) {
    LARGE_INTEGER frequency, start, end;
    double seconds;

    int release = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--release") == 0) {
            release = 1;
        }
    }

    if (!QueryPerformanceFrequency(&frequency)) {
        fprintf(stderr, "Failed to get performance counter frequency\n");
        return 1;
    }

    if (!QueryPerformanceCounter(&start)) {
        fprintf(stderr, "Failed to get starting time\n");
        return 1;
    }

    char buf[512] = {0};
    const char *optimization = release ? "/O2" : "/Zi";
    sprintf(buf, "%s %s %s src\\main.c -Fe:_main.exe -link %s %s", CC, CFLAGS, optimization, LINK_ARGS, LIBS);
    printf("%s\n", buf);
    system(buf);

    system("del /Q .\\*.obj");
    system("del /Q .\\*.ilk");
    
    if (!QueryPerformanceCounter(&end)) {
        fprintf(stderr, "Failed to get ending time\n");
        return 1;
    }

    seconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Time taken: %.6f seconds\n", seconds);
    return 0;
}
