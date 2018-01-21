#include "pwr5.h"

#define PWR5_VERSION "0.1beta"

int usage() {
	printf("pwr5 - get your laptop power fine\n
version: %s\n
using libpwr5 version: %s\n
--
Usage: %s [subcommand] [options]\n
* Available sub commands
status      to show current power status
scheme      power scheme option
");
}

int main(int argc, char *argv[]) {
	return 0;
}
