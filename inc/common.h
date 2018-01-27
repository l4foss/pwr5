#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>
#include <stdint.h>

#define PWR5_VERSION 0
#define PWR5_SUBVERSION 1
#define PWR5_PATCH 0

#define ulong_t unsigned long
#define ullong_t unsigned long long

#define SYSFS_MAX_PATH 255

#define TERM_RED    "\x1b[31;1m"
#define TERM_GREEN  "\x1b[32;1m"
#define TERM_YELLOW "\x1b[33;1m"
#define TERM_BLUE   "\x1b[34;1m"
#define TERM_CYAN   "\x1b[36;1m"
#define TERM_WHITE  "\x1b[1m"
#define TERM_COLOR_X "\x1b[m"

#ifdef NDEBUG
#define PWR5_DEBUG(MSG)
#else
#define PWR5_DEBUG(MSG) \
	do { fprintf(stderr, TERM_WHITE \
		             "[DEBUG]" TERM_COLOR_X "(%s:%d:%s) %s\n", __FILE__, \
		             __LINE__, __func__, MSG); } while (0)

#define PWR5_DEBUGA(fmt, ...) \
	do { \
		fprintf(stderr, TERM_WHITE "[DEBUG]" TERM_COLOR_X "(%s:%d:%s) " fmt, \
		        __FILE__, __LINE__, __func__, \
		        __VA_ARGS__);} while (0)

#endif


#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define PWR5_ERR(MSG) \
	do { \
		fprintf(stderr, TERM_RED \
		        "[ERROR]" TERM_COLOR_X "(%s:%d:%s, errno: %s) %s\n", \
		        __FILE__, __LINE__, __func__, clean_errno(), MSG);\
	} while (0)

#define PWR5_WARN(MSG) \
	do { \
		fprintf(stderr, TERM_YELLOW \
		        "[WARN]" TERM_COLOR_X "(%s:%d:%s, errno: %s) %s\n", \
		        __FILE__, __LINE__, __func__, clean_errno(), MSG);\
	} while (0)

#define PWR5_INFO(MSG) \
	do { fprintf(stderr, TERM_GREEN \
		             "[INFO]" TERM_COLOR_X " (%s:%d:%s) %s\n", \
		             __FILE__, __LINE__, __func__, MSG); \
	} while (0)

#endif
