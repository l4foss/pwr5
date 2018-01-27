#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include "common.h"
#include "sysfsutil.h"
#include <dirent.h>

/* 
 * XCB headers as fallback in case of sysfs interface does
 * not work
 */
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>
#include <xcb/randr.h>

#define BACKLIGHT_XCB_NEW "Backlight"
#define BACKLIGHT_XCB_LEGACY "BACKLIGHT"

#define BACKLIGHT_PATH "/sys/class/backlight/"
#define MAX_BACKLIGHT_NAME 50

enum bltype {
	BACKLIGHT_TYPE_UNKNOWN,
	BACKLIGHT_TYPE_HARDWARE,
	BACKLIGHT_TYPE_SOFTWARE,
};

enum backlight_prop {
	BACKLIGHT_INFO_MAX,
	BACKLIGHT_INFO_NOW,

	BACKLIGHT_TOTAL_ENTRY
};

typedef struct {
	int type;
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
