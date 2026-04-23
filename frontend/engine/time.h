#ifndef TIME_H
#define TIME_H

#include <stdint.h>

static uint32_t time_ms = 1000;

#ifdef _WIN32
#  include <windows.h>
static uint64_t get_now(void) {
    return GetTickCount64();
}
static void sleep_until(uint64_t start) {
    uint64_t end = start + time_ms;
    uint64_t now = GetTickCount64();
    if (end > now) Sleep((uint32_t)(end - now));
}
#else
#  include <time.h>
static struct timespec get_now(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t;
}
static void sleep_until(struct timespec start) {
    struct timespec end = start;
    end.tv_sec  +=  time_ms / 1000;
    end.tv_nsec += (time_ms % 1000) * 1000000L;
    if (end.tv_nsec >= 1000000000L) { end.tv_sec++; end.tv_nsec -= 1000000000L; }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &end, NULL);
}
#endif

#endif /* TIME_H */
