#ifndef TIME_H
#define TIME_H
#include <stdint.h>

extern uint32_t time_ms;

#ifdef _WIN32
  typedef uint64_t Timestamp;
#else
  #include <time.h>
  typedef struct timespec Timestamp;
#endif

Timestamp get_now(void);
void sleep_until(Timestamp start);

#endif
