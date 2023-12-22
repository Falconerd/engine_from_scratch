#include <windows.h>
#include <stdio.h>
#include <string.h>
#define CFLAGS "-nologo -GR- -Oi"
#define LIBPATH ".\\lib"
#define CC "cl"

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
    sprintf(buf, "%s %s %s src\\main.c -Fe:_main.exe -link -nodefaultlib -subsystem:windows -libpath:%s kernel32.lib user32.lib gdi32.lib", CC, CFLAGS, optimization, LIBPATH);
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
