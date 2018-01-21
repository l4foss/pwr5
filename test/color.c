#include "common.h"

int main() {
	PWR5_DEBUGA("shit %d\n", 1214);
	PWR5_WARN("shit");
	PWR5_ERR("shit");
	PWR5_INFO("shit");
	PWR5_DEBUG(TERM_CYAN "shit" TERM_COLOR_X);
	PWR5_DEBUG("shit");
}
