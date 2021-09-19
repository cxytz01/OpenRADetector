#include <cstdio>

inline void color_test() {
	printf("\033[30m -- black --\033[0m\n");
	printf("\033[31m -- red --\033[0m\n");
	printf("\033[32m -- green --\033[0m\n");
	printf("\033[33m -- yellow --\033[0m\n");
	printf("\033[34m -- blue --\033[0m\n");
	printf("\033[35m -- magenta --\033[0m\n");
	printf("\033[36m -- cyan --\033[0m\n");
	printf("\033[37m -- light gray --\033[0m\n");
	printf("\033[90m -- dark gray --\033[0m\n");
	printf("\033[91m -- light red --\033[0m\n");
	printf("\033[92m -- light green --\033[0m\n");
	printf("\033[93m -- light yellow --\033[0m\n");
	printf("\033[94m -- light blue --\033[0m\n");
	printf("\033[95m -- light magenta --\033[0m\n");
	printf("\033[96m -- light cyan --\033[0m\n");
	printf("\033[97m -- white --\033[0m\n");
	printf("\033[1m -- bold --\033[0m\n");
	printf("\033[4m -- underline --\033[0m\n");
	printf("\033[24m -- no underline --\033[0m\n");
	printf("\033[7m -- Negative(reverse the foreground and background) --\033[0m\n");
	printf("\033[27m -- Positive(no negative) --\033[0m\n");
}