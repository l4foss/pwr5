#ifndef UTIL_H
#define UTIL_H

#include "common.h"
#include <fcntl.h>

#define MAX_LINE_LEN 1024

ssize_t util_write_buf_to_sysfs(char *, char *, size_t);
char *util_read_buf_from_sysfs(char *, size_t);

ulong_t util_read_ulong_from_sysfs(char *);
ssize_t util_write_ulong_to_sysfs(char *, ulong_t);

#endif
