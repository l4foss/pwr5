/*
 * this contains some helper funtions to get cpuinfo, using sysfs
 * as well as to modify some cpu parameters, especially power parameters
 * including cpu governer and scaling frequency
 */

#ifndef CPU_H
#define CPU_H

#include "common.h"

#define CPU_PATH "/sys/devices/system/cpu/"
#define CPU_DRIVER_MAX 127
#define CPU_LINE_MAX 255

/*
 * others with be added later
 */
enum arch {
	CPU_ARCH_X86,
	CPU_ARCH_X86_64,
	CPU_ARCH_ARM,
	CPU_ARCH_ARM64
};

/*
 * common governers for intel and arm cpus
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
	char driver[CPU_DRIVER_MAX];
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

/*
 * @brief CPU info
 */
typedef struct {
	char model[CPU_MAX_LINE];
	char manufacturer[CPU_MAX_LINE];
	int arch;

	FreqInfo freq;
	ScalingInfo scaling;
	GovernerInfo governer;
	PstateInfo *pstate;

} CPUInfo;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get cpu info
 * @param cpu number
 * @return a pointer to CPUInfo
 * @return NULL if cpu does not exist
 */
CPUInfo *cpu_get_info(int);

/**
 * @brief Get CPU governer info
 * @param cpu number
 * @return a pointer to GovernerInfo
 * @return NULL if cpu does not exist
 */
GovernerInfo *cpu_get_governer_info(int);

/**
 * @brief Get CPU scaling info
 * @param cpu number
 * @return a pointer to ScalingInfo
 * @return NULL if cpu does not exist
 */
ScalingInfo *cpu_get_scaling_info(int);

/**
 * @brief Get CPU frequency info
 * @param cpu number
 * @return a pointer to FreqInfo
 * @return NULL if cpu does not exist
 */
FreqInfo *cpu_get_freq_info(int);

/**
 * @brief Set CPU governer
 * @param cpu number
 * @param governer
 * @return 0 if success
 * @return -1 if governer does not available
 */
int cpu_set_governer(int, int);

/**
 * @brief Set CPU scaling max frequency
 * @param cpu number
 * @param frequency
 * return 0 if success
 * return -1 if failed
 */
int cpu_set_scaling_max(int, ulong_t);

/**
 * @brief Set CPU scaling min frequency
 * @param cpu number
 * @param frequency
 * @return 0 if success
 * @return -1 if failed
 */
int cpu_set_scaling_min(int, ulong_t)
#ifdef __cplusplus
}
#endif

#endif
