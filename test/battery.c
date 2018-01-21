#include "battery.h"

int main() {
	PowerSourceInfo *pi = battery_detect();

	printf("%d\n%d\n%d\n", pi->source,
			pi->bat, pi->ac);

	BatteryInfo *bi = battery_get_info(1);

}
