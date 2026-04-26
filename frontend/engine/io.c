#include "io.h"

#ifdef _WIN32
#include <windows.h>

int32_t stdin_bytes_available(void) {
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    DWORD avail = 0;
    if (!PeekNamedPipe(h, NULL, 0, NULL, &avail, NULL))
        return -1;
    return (int32_t)avail;
}

#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h> // Sometimes required on older Posix systems

int32_t stdin_bytes_available(void) {
    int bytes;
    // FIONREAD is widely supported across BSD, Linux, and macOS
    if (ioctl(STDIN_FILENO, FIONREAD, &bytes) < 0) {
        return -1;
    }
    return (int32_t)bytes;
}

#endif
