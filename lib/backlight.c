#include "backlight.h"

static BacklightInfo *backlight_get_info_xrandr(void);
static ulong_t backlight_read_property_ulong(char *, char *);

static const char *backlight_prop_file[BACKLIGHT_TOTAL_ENTRY] = {
	[BACKLIGHT_INFO_MAX] = "max_brightness",
	[BACKLIGHT_INFO_NOW] = "brightness"
};

/*
 * first, try to get backlight via sysfs interface
 * if it does not work, check if X is available, then
 * call xrandr to get backlight as fallback case
 * which is slower but works in almost all devices
 *
 * if X is not available, return NULL ツ
 */
BacklightInfo *backlight_get_info(void) {
	BacklightInfo *bl = malloc(sizeof(BacklightInfo));
	if (!bl) {
#ifdef DEBUG_ON
	PWR5_DEBUG("NULL pointer due to memory allocation failed!");
#endif
		goto error;
	}

	DIR *dir;
	struct dirent *de;

	dir = opendir(BACKLIGHT_PATH);
	if (!dir) {
		goto fallback;
	}

	char *name = NULL;
	while (1) {
		de = readdir(dir);
		if (!de) {
			break;
		}

		/* NOTE: this works by choosing the first entry
		 * TODO: fix this */
		name = malloc(sizeof(de->d_name)+1);
		if (!name) {
			goto error;
		}
		strcpy(name, de->d_name);
		break;
	}
	closedir(dir);

	/* sysfs */
#ifdef DEBUG_ON
	PWR5_DEBUGA("name: %s\n", name);
#endif
	strcpy(bl->name, name);
	free(name);

	/* read properties from sysfs dir */
	bl->max = backlight_read_property_ulong(bl->name,
			backlight_prop_file[BACKLIGHT_INFO_MAX]);

	bl->now = backlight_read_property_ulong(bl->name,
			backlight_prop_file[BACKLIGHT_INFO_NOW]);

	if (bl->max == 0) {
		goto fallback;
	}
	/* calculate the min value 10% of max */
	bl->min = (ulong_t)(bl->max/10l);
	return bl;

fallback:
	PWR5_WARN("Failed to get backlight via sysfs, falling back to xrandr!");
	return backlight_get_info_xrandr();

error:

	return NULL;
}

/*
 * get backlight from sysfs interface
 */
static ulong_t backlight_read_property_ulong(char *name, char *prop) {
	char path[SYSFS_MAX_PATH];
	snprintf(path, SYSFS_MAX_PATH, BACKLIGHT_PATH "%s/%s", name, prop);
#ifdef DEBUG_ON
	PWR5_DEBUGA("path: %s\n", path);
#endif
	ulong_t ret = util_read_ulong_from_sysfs(path);
	return ret;
}

/*
 * get backlight info via xrandr
 */
static BacklightInfo *backlight_get_info_xrandr(void) {
	struct xcb_connection_t *conn;
	struct xcb_randr_output_t output;

}


