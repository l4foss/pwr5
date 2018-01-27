#include "battery.h"
#include "sysfsutil.h"

static const char *bat_prop_file[BAT_INFO_TOTAL_ENTRY] = {
	[BAT_INFO_SERIAL_NUMBER] = "serial_number",
	[BAT_INFO_MANUFACTURER] = "manufacturer",
	[BAT_INFO_MODEL_NAME] = "model_name",
	[BAT_INFO_TECHNOLOGY] = "technology",
	[BAT_INFO_CYCLE_COUNT] = "cycle_count",
	[BAT_INFO_ENERGY_FULL_DESIGN] = "energy_full_design",
	[BAT_INFO_ENERGY_FULL] = "energy_full",
	[BAT_INFO_ENERGY_NOW] = "energy_now",
	[BAT_INFO_VOLTAGE_MIN_DESIGN] = "voltage_min_design",
	[BAT_INFO_VOLTAGE_NOW] = "voltage_now",
	[BAT_INFO_POWER_NOW] = "power_now",
	[BAT_INFO_CAPACITY] = "capacity",
	[BAT_INFO_CAPACITY_LEVEL] = "capacity_level",
	[BAT_INFO_STATUS] = "status"
};

static char *battery_read_property_bufline(int bat, const char *prop) {
	char path[SYSFS_MAX_PATH];
	snprintf(path, sizeof(path), BATTERY_PATH "BAT%d/%s", bat, prop);
	char *buf = util_read_buf_from_sysfs(path, MAX_LINE_LEN);
#ifdef DEBUG_ON
	printf("%s", buf);
#endif
	return buf;
}

static ulong_t battery_read_property_ulong(int bat, const char *prop) {
	char path[SYSFS_MAX_PATH];
	snprintf(path, sizeof(path), BATTERY_PATH "BAT%d/%s", bat, prop);
	return util_read_ulong_from_sysfs(path);
}

BatteryInfo *battery_get_info(int bat) {
	char *buf;
	BatteryInfo *binfo = malloc(sizeof(BatteryInfo));
	if (!binfo) {
		return NULL;
	}

	bzero(binfo, 0x00);
	binfo->num = bat;
	/* Manufacturer */
	buf = battery_read_property_bufline(bat,
	                                    bat_prop_file[BAT_INFO_MANUFACTURER]);
	memcpy(binfo->manufacturer, buf, strlen(buf));
	free(buf);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got manufacturer %s\n", binfo->manufacturer);
#endif
	buf = battery_read_property_bufline(bat,
	                                    bat_prop_file[BAT_INFO_SERIAL_NUMBER]);
	memcpy(binfo->serialNumber, buf, strlen(buf));
	free(buf);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got serial number %s\n", binfo->serialNumber);
#endif
	buf = battery_read_property_bufline(bat, bat_prop_file[BAT_INFO_MODEL_NAME]);
	memcpy(binfo->modelName, buf, strlen(buf));
	free(buf);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got model name %s\n", binfo->modelName);
#endif
	buf = battery_read_property_bufline(bat, bat_prop_file[BAT_INFO_TECHNOLOGY]);
	memcpy(binfo->technology, buf, strlen(buf));
	free(buf);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got technology %s\n", binfo->technology);
#endif
	/* some long ints */
	binfo->cycleCount = battery_read_property_ulong(bat,
	                    bat_prop_file[BAT_INFO_CYCLE_COUNT]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got cycle count %d\n", binfo->cycleCount);
#endif
	binfo->energyFullDesign = battery_read_property_ulong(bat,
	                          bat_prop_file[BAT_INFO_ENERGY_FULL_DESIGN]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got energy full design %d\n", binfo->energyFullDesign);
#endif
	binfo->energyFull = battery_read_property_ulong(bat,
	                    bat_prop_file[BAT_INFO_ENERGY_FULL]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got energy_full %d\n", binfo->energyFull);
#endif
	binfo->energyNow = battery_read_property_ulong(bat,
	                   bat_prop_file[BAT_INFO_ENERGY_NOW]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got energy_now %d\n", binfo->energyNow);
#endif
	binfo->voltageMinDesign = battery_read_property_ulong(bat,
	                          bat_prop_file[BAT_INFO_VOLTAGE_MIN_DESIGN]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got voltage_min_design %d\n", binfo->voltageMinDesign);
#endif
	binfo->voltageNow = battery_read_property_ulong(bat,
	                    bat_prop_file[BAT_INFO_VOLTAGE_NOW]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got voltage_now %d\n", binfo->voltageNow);
#endif
	binfo->powerNow = battery_read_property_ulong(bat,
	                  bat_prop_file[BAT_INFO_POWER_NOW]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got power_now %d\n", binfo->powerNow);
#endif
	binfo->capacity = battery_read_property_ulong(bat,
	                  bat_prop_file[BAT_INFO_CAPACITY]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got capacity %d\n", binfo->capacity);
#endif
	binfo->capacityLevel = battery_read_property_ulong(bat,
	                       bat_prop_file[BAT_INFO_CAPACITY_LEVEL]);
#ifdef DEBUG_ON
	PWR5_DEBUGA("Got capacity_level %d\n", binfo->capacityLevel);
#endif
	/* battery status */
	buf = battery_read_property_bufline(bat,
	                                    bat_prop_file[BAT_INFO_STATUS]);
	if (strcmp(buf, BAT_ST_CHARGING_S) == 0) {
		binfo->status = BAT_ST_CHARGING;
	} else if (strcmp(buf, BAT_ST_FULL_S) == 0) {
		binfo->status = BAT_ST_FULL;
	} else if (strcmp(buf, BAT_ST_USING_S) == 0) {
		binfo->status = BAT_ST_USING;
	} else if (strcmp(buf, BAT_ST_UNKNOWN_S) == 0) {
		binfo->status = BAT_ST_UNKNOWN;
	} else {
		binfo->status = BAT_ST_UNKNOWN;
	}
#ifdef DEBUG_ON
	PWR5_DEBUGA("status: %s\n", buf);
#endif
	free(buf);
	return binfo;
}

BatAPI *batapi_get_info(int bat) {
	return NULL;
}

PowerSourceInfo *battery_detect(void) {
	DIR *dir = opendir(BATTERY_PATH);
	if (!dir) {
		return NULL;
	}
	PowerSourceInfo *pi = malloc(sizeof(PowerSourceInfo));
	if (!pi) {
		closedir(dir);
	}
	struct dirent *de;
	while (1) {
		de = readdir(dir);
		if (!de) {
			break;
		}
		if (memcmp(de->d_name, "BAT", 3) == 0) {
			++pi->bat;
		} else if (memcmp(de->d_name, "AC", 2) == 0) {
			++pi->ac;
			pi->source = SOURCE_AC;
		}
	}
#if DEBUG_ON
	PWR5_DEBUGA("Detected %d batteries\n", pi->bat);
	PWR5_DEBUGA("Detected %d AC adapters\n", pi->ac);
#endif
	//fallback to error
	if (pi->bat == 0 &&
	        pi->ac == 0) {
		goto error;
	}
	return pi;
error:
	free(pi);
	closedir(dir);
	return NULL;
}
