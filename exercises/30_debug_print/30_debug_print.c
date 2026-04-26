#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#endif

#if DEBUG_LEVEL >= 1
#define DEBUG_PRINT(fmt, ...) do { \
    printf("DEBUG: func=%s, line=%d", __func__, __LINE__); \
    if (DEBUG_LEVEL >= 2) { \
        printf(", " fmt, ##__VA_ARGS__); \
    } \
    printf("\n"); \
    if (DEBUG_LEVEL >= 3) { \
        void *callstack[10]; \
        int frames = backtrace(callstack, 10); \
        char **symbols = backtrace_symbols(callstack, frames); \
        if (symbols) { \
            for (int i = 1; i < frames; i++) { \
                printf("DEBUG: Caller: %s\n", symbols[i]); \
            } \
            free(symbols); \
        } \
    } \
} while (0)
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif















void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}
