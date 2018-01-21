#include "common.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

enum {
	C_RED,
	C_GREEN,
	C_BLUE,
	C_MAGENTA,
	C_YELLOW,
	C_CYAN,
	C_WHITE
};



int fancy_print(char *str, int color) {
	if (!isatty(1)) {
		
	}

	switch (color) {
		case C_RED:
			printf(RED "%s" RESET, str);
		case C_GREEN:
			printf(GRN "%s" RESET, str);
		case C_YELLOW:
			printf(YEL "%s" RESET, str);
		case C_BLUE:
			printf(BLU "%s" RESET, str);
		case C_MAGENTA:
			printf(MAG "%s" RESET, str);
		case C_CYAN:
			printf(CYN "%s" RESET, str);
		case C_WHITE:
			printf(WHT "%s" RESET, str);
	}
	return 0;
}

int basic_info(char *prog) {
	printf("pwr5 status
			Power source: %s\n
			Current scheme: %s\n");
}

int util_is_root() {

}
