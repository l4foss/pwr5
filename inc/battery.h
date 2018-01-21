#ifndef BATTERY_H
#define BATTERY_H

#include "common.h"
#include <dirent.h>

#define BATTERY_PATH "/sys/class/power_supply/"

#define BAT_ST_CHARGING_S "Charging"
#define BAT_ST_FULL_S     "Full"
#define BAT_ST_USING_S    "Discharging"
#define BAT_ST_UNKNOWN_S  "Unknown"
/**
 * @brief Errors
 */
enum error {
	BAT_ERR_UNKNOWN,
	BAT_ERR_NOMEM,
	BAT_ERR_NO_BATAPI
};

/**
 * @brief Power source
 */
enum source {
	SOURCE_UNKNOWN,
	SOURCE_AC,
	SOURCE_BAT
};

/*
 * @brief Battery status
 */
enum status {
	BAT_ST_CHARGING,
	BAT_ST_FULL,
	BAT_ST_USING,
	BAT_ST_UNKNOWN
};

/**
 * @brief Battery API (for Thinkpad laptops)
 * @note This works by reading value from ACPI, which is supported by acpi_call
 * this could be working, could be not, depends on your device
 */
typedef struct {
	uint32_t startThreshold; /**< Where to start charging*/
	uint32_t stopThreshold; /**< where to stop charging*/
	int forceDischarge;		/**< force discharge the battery*/
} BatAPI;

/**
 * @brief sysfs files that contain 
 * battery info
 */
enum sysfs_file {
	BAT_INFO_SERIAL_NUMBER,
	BAT_INFO_MANUFACTURER,
	BAT_INFO_MODEL_NAME,
	BAT_INFO_TECHNOLOGY,
	BAT_INFO_CYCLE_COUNT,
	BAT_INFO_ENERGY_FULL_DESIGN,
	BAT_INFO_ENERGY_FULL,
	BAT_INFO_ENERGY_NOW,
	BAT_INFO_VOLTAGE_MIN_DESIGN,
	BAT_INFO_VOLTAGE_NOW,
	BAT_INFO_POWER_NOW,
	BAT_INFO_CAPACITY,
	BAT_INFO_CAPACITY_LEVEL,
	BAT_INFO_STATUS,

	BAT_INFO_TOTAL_ENTRIES
};

/**
 * @brief Battery info
 * contains all info about battery
 */
typedef struct {
	int num;
	int status;		/** status string */

	char serialNumber[20];
	char manufacturer[40];  /**< Manufacturer */
	char modelName[40];		/**< Battery model name */
	char technology[20];	/**< Technology, should be either Li-ion or Li-Po */
	
	uint32_t cycleCount;	/**< number of cycle, 0 if unsupported */
	uint32_t energyFullDesign; /** energy when battery is full (design) */
	uint32_t energyFull;		/** energy when the battery is full (current) */
	uint32_t energyNow;			/** energy at the moment */
	uint32_t voltageMinDesign; /** minimum voltage by design */
	uint32_t voltageNow;		/** current voltage */
	uint32_t powerNow;			/** current power */
	uint32_t capacity;			/** battery capacity */
	uint32_t capacityLevel;		/** capacity level */

	BatAPI *batapi;			/** battery api, NULL if unsupported */
} BatteryInfo;

typedef struct {
	int		source; /** current power source */
	uint32_t bat;  /** number of battery available */
	uint32_t ac;   /** number of ac power available */
} PowerSourceInfo;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get all data about a battery
 * @return a pointer to a BatteryInfo struct
 * @return NULL if no such battery
 * @p battery id, for example your battery is BAT0, then it would be 0
 */
BatteryInfo *battery_get_info(int);

/**
 * @brief Get battery api for thinkpad laptops
 * @p battery id
 * @return a pointer to BatAPI struct
 */
BatAPI *batapi_get_info(int);

/**
 * @brief Set battery charging start threshold
 * @p battry number
 * @p value
 * @p temp or permanently
 * @return 0 if successfully set
 * @return 1 if failed
 */
int batapi_set_start_threshold(int, uint32_t, int);

/**
 * @brief Set battery stop charging threshold
 * @p battery number
 * @p value
 * @p temp or permanently
 * @return 0 if successfully set
 * @return 1 if any error
 */
int batapi_set_stop_threshold(int, uint32_t, int);

/**
 * @brief Reset battery charge threshold
 * @p battery number
 */
int batapi_reset_threshold(int);

/**
 * @brief Force discharge battery
 * @p battery number
 * @return 0 in case of success
 * @return 1 if any error
 */
int batapi_force_discharge(int);

/**
 * @brief Recalibrate the battery
 * @p battery number
 */
int batapi_recalibrate(int);

/**
 * @brief Detects batteries if any
 * @return an array of battery nums
 * @return NULL if no battery detected
 */
PowerSourceInfo *battery_detect(void);

#ifdef __cplusplus
}
#endif

#endif
