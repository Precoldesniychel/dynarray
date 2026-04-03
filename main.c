#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void run_interface(void);

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        #ifdef _WIN32
        return system("build\\tests.exe");
        #else
        return system("./build/tests");
        #endif
    }
    run_interface();
    return 0;
}