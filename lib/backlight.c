#include "backlight.h"

/*
 * reads backlight info from xrandr */
static BacklightInfo *backlight_get_info_xrandr(void);
/* reads backlight info from sysfs */
static ulong_t backlight_read_property_ulong(char *, const char *);
/*
 * get output from xrandr */
static xcb_randr_output_t get_output(xcb_connection_t *);

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

	bl->curr = backlight_read_property_ulong(bl->name,
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
	PWR5_ERR("Could not get backlight info");
	return NULL;
}

/*
 * get the 1st output
 */
static xcb_randr_output_t get_output(xcb_connection_t *conn) {
	xcb_randr_get_screen_resources_cookie_t resources_cookie;
	xcb_randr_get_screen_resources_reply_t *resources_reply;
	xcb_generic_error_t *error;

	xcb_screen_t *screen;
	xcb_window_t root;
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator (xcb_get_setup (conn));
	while (iter.rem) {
		screen = iter.data;
		root = screen->root;

		resources_cookie = xcb_randr_get_screen_resources (conn, root);
		resources_reply = xcb_randr_get_screen_resources_reply (conn, resources_cookie, &error);

		if (error != NULL ||
				resources_reply == NULL) {
			printf("Error: %d\n", error->error_code);
			continue;
		}
		//the first output
		xcb_randr_output_t *outputs = xcb_randr_get_screen_resources_outputs(resources_reply);
		return outputs[0];
	}
}

/*
 * get backlight from sysfs interface
 */
static ulong_t backlight_read_property_ulong(char *name, const char *prop) {
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
	xcb_atom_t backlight;
	xcb_connection_t *conn;
	//xcb_randr_output_t output;
	xcb_intern_atom_cookie_t cookie;
	xcb_intern_atom_reply_t *reply;
	xcb_generic_error_t *error;


	/* This gets the first display, it mays not working properly
	 * if you have more than one display
	 * TODO: fix this
	 */
	conn = xcb_connect(NULL, NULL);
	if (!conn) {
		return NULL;
	}

	cookie = xcb_intern_atom(conn, 1, strlen(BACKLIGHT_XCB_NEW),
			BACKLIGHT_XCB_NEW);
	reply = xcb_intern_atom_reply(conn, cookie, &error);
	if (error != NULL ||
			reply == NULL) {
		goto error;
	}
	backlight = reply->atom;

	if (backlight == XCB_NONE) {
		cookie = xcb_intern_atom(conn, 1, strlen(BACKLIGHT_XCB_NEW),
				BACKLIGHT_XCB_NEW);
		reply = xcb_intern_atom_reply(conn, cookie, &error);
		if (error != NULL ||
				reply == NULL) {
			goto error;
		}
		backlight = reply->atom;
	}

	if (backlight == XCB_NONE) {
		goto error;
	}


	BacklightInfo *binfo = malloc(sizeof(BacklightInfo));
	if (!binfo) {
		goto error;
	}

error:
	free(reply);
	free(conn);
	return NULL;
}


