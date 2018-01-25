#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

#define SYSTEM_DMI_PATH "/sys/devices/virtual/dmi/id"
#define MAX_VERDOR_S 255
#define MAX_MODEL_S 255
#define MAX_VERSION_S 255

enum {
	SYSTEM_BOOT_UNKNOWN,
	SYSTEM_BOOT_LEGACY,
	SYSTEM_BOOT_UEFI
};

typedef struct {
	int bootMethod;
	char biosVendor[MAX_VERDOR_S];
	char biosVersion[MAX_VERSION_S]
} BootInfo;

typedef struct {
	char vendor[MAX_VERDOR_S];
	char model[MAX_MODEL_S];
	BootInfo boot;
} SystemInfo;

#ifdef __cplusplus
extern "C" {
#endif

BootInfo *system_get_boot_info();
SystemInfo *system_get_info();

#ifdef __cplusplus
}
#endif

#endif
