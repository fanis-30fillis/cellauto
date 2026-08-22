#include <windows.h>
#include <stdint.h>
#include "time.h"

uint64_t monotonic_ns() {
    static LARGE_INTEGER frequency;
    static int initialized = 0;

    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (uint64_t)(
        (counter.QuadPart * 1000000000ULL) / frequency.QuadPart
        );
}