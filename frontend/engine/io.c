#include "io.h"

#ifdef _WIN32
#include <windows.h>
#include <string.h>
#include <stdio.h>

int32_t stdin_bytes_available(void) {
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    DWORD avail = 0;
    if (!PeekNamedPipe(h, NULL, 0, NULL, &avail, NULL))
        return -1;
    return (int32_t)avail;
}

int32_t get_exe_directory(char *out_path, size_t out_size) {
    if (!out_path || out_size == 0) return -1;
    DWORD len = GetModuleFileNameA(NULL, out_path, (DWORD)out_size);
    if (len == 0 || len >= out_size) return -1;
    char *last_slash = strrchr(out_path, '\\');
    if (last_slash) *last_slash = '\0';
    else { out_path[0] = '.'; out_path[1] = '\0'; }
    return 0;
}

#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int32_t stdin_bytes_available(void) {
    int bytes;
    if (ioctl(STDIN_FILENO, FIONREAD, &bytes) < 0) return -1;
    return (int32_t)bytes;
}

int32_t get_exe_directory(char *out_path, size_t out_size) {
    if (!out_path || out_size == 0) return -1;
    ssize_t len = readlink("/proc/self/exe", out_path, out_size - 1);
    if (len == -1) return -1;
    out_path[len] = '\0';
    char *last_slash = strrchr(out_path, '/');
    if (last_slash) *last_slash = '\0';
    return 0;
}

#endif

int32_t get_exe_file_path(const char *relative_path, char *out_path, size_t out_size) {
    char exe_dir[1024];

    if (!relative_path || !out_path || out_size == 0)
        return -1;

    if (get_exe_directory(exe_dir, sizeof(exe_dir)) != 0)
        return -1;

    snprintf(out_path, out_size, "%s/%s", exe_dir, relative_path);
    return 0;
}

int32_t read_file_from_exe_dir(const char *relative_path, void *buffer, size_t buffer_size) {
    char full_path[2048];

    if (get_exe_file_path(relative_path, full_path, sizeof(full_path)) != 0)
        return -1;  // Failed to get path

    FILE *f = fopen(full_path, "rb");
    if (!f)
        return -2;  // Failed to open

    size_t bytes_read = fread(buffer, 1, buffer_size, f);
    fclose(f);

    return (int32_t)bytes_read;  // Return bytes read, -1 or -2 on error
}
