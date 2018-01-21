#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include "common.h"
#include <dirent.h>

/*
 * TODO: use software interface (xrandr) as a fallback
 * if case of sysfs interface does not work
 */
#define BACKLIGHT_PATH "/sys/class/backlight/"
#define MAX_BACKLIGHT_NAME 50

typedef struct {
	uint32_t curr;
	uint32_t min;
	uint32_t max;
	char name[MAX_BACKLIGHT_NAME];
} BacklightInfo;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get backlight info
 * @return a pointer to BacklightInfo
 */
BacklightInfo *backlight_get_info(void);

/**
 * @brief Set brightness, by write the value directly into sysfs path
 * @p backlight name
 * @p backlight value
 */
int backlight_set_raw(const char *, int);

/**
 * @brief Set brightness 
 * @note since this value may cause your screen completely
 * off, this makes sure that your screen is on by measuring a minimal value of
 * brightness
 */
int backlight_set_percent(const char *, int);

/**
 * @brief Set brightness, safe to use but use use raw value
 * instead of percentage
 */
int backlight_set_int(const char *, int);

#ifdef __cplusplus
}
#endif

#endif
