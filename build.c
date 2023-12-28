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
#define CFLAGS "-TC -nologo -GR- -GS- -Gs1000000000 -Oi -W4 -MP"
#define CC "cl"

/*
    -nodefaultlib: Don't link CRT or auto-link kernel32 etc.
    -subsytem:windows: Create a windows application (not console).
    -stack...: Commit a larger stack.
*/
#define LINK_ARGS "-nodefaultlib -subsystem:windows -stack:0x200000,200000"
#define LIBS "kernel32.lib user32.lib gdi32.lib opengl32.lib"

int main(int argc, char *argv[]) {
    LARGE_INTEGER frequency, start, end;
    double seconds;

    int release = 0;
    int link = 1;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--release") || !strcmp(argv[i], "-r")) {
            release = 1;
        }
        if (!strcmp(argv[i], "-c")) {
            link = 0;
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
    const char *release_flags = release ? "-DBUILD_SPEED -DBUILD_EXTERNAL" : "";

    char link_str[256] = {'-', 'c'};
    if (link) {
        sprintf(link_str, "-link %s %s", LINK_ARGS, LIBS);
    }

    // Always do a full build and link.
    system("del /Q _main.ilk");
    system("del /Q _main.pdb");

    sprintf(buf, "%s %s %s %s src\\win32_main.c -Fe:_main.exe %s", CC, CFLAGS, optimization, release_flags, link_str);
    printf("%s\n", buf);
    system(buf);

    if (!QueryPerformanceCounter(&end)) {
        fprintf(stderr, "Failed to get ending time\n");
        return 1;
    }

    seconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Time taken: %.6f seconds\n", seconds);
    return 0;
}

