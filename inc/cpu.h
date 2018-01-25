/*
 * this contains some helper funtions to get cpuinfo, using sysfs
 * as well as to modify some cpu parameters, especially power parameters
 * including cpu governer and scaling frequency
 */

#ifndef CPU_H
#define CPU_H

#include "common.h"

#define CPU_PATH "/sys/devices/system/cpu/"

enum arch {
	CPU_ARCH_X86,
	CPU_ARCH_X86_64,
	CPU_ARCH_ARM,
	CPU_ARCH_ARM64
};

/*
 * common governer for intel x86_64 and arm cpus
 */
enum governer {
	CPU_GOVERNER_POWERSAVE = 1,
	CPU_GOVERNER_PERFORMANCE = 1 << 1,
	CPU_GOVERNER_CONSERVATIVE = 1 << 2,
	CPU_GOVERNER_USERSPACE = 1 << 3,
	CPU_GOVERNER_SCHEDUTIL = 1 << 4,
	CPU_GOVERNER_ONDEMAND  = 1 << 5
};

typedef struct {
	ulong_t min;
	ulong_t max;
} FreqInfo;

typedef struct {
	ulong_t min;
	ulong_t max;
	char *driver;
	ulong_t latency;
} ScalingInfo;

typedef struct {
	uint32_t available;
	uint32_t current;
} GovernerInfo;

/*
 * for intel only
 */
typedef struct {
	ulong_t min;
	ulong_t max;
	int turbo;
	ulong_t num;
	ulong_t turbo_pct;
} PstateInfo;

typedef struct {
	char model[100];
	char manufacturer[50];

	FreqInfo freq;
	ScalingInfo scaling;
	GovernerInfo governer;
	PstateInfo *pstate;

} CPUInfo;

#ifdef __cplusplus
extern "C" {
#endif

CPUInfo *cpu_get_info(int);
GovernerInfo *cpu_get_governer_info(int);
ScalingInfo *cpu_get_scaling_info(int);
FreqInfo *cpu_get_freq_info(int);

int cpu_set_governer(int, int);
int cpu_set_scaling_max(int, ulong_t);
int cpu_set_scaling_min(int, ulong_t)
#ifdef __cplusplus
}
#endif

#endif
