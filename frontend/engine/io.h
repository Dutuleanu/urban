#ifndef IO_H
#define IO_H
#include <stdint.h>
#include <stddef.h>

int32_t stdin_bytes_available(void);
int32_t get_exe_directory(char *out_path, size_t out_size);
int32_t get_exe_file_path(const char *relative_path, char *out_path, size_t out_size);
int32_t read_file_from_exe_dir(const char *relative_path, void *buffer, size_t buffer_size);

#endif
