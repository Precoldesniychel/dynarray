#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void run_interface(void);

int main(int argc, char* argv[]) {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        printf("Запустите тесты отдельно: .\\Release\\run_tests.exe\n");
        return 0;
    }

    run_interface();
    return 0;
}