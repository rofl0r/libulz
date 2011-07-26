#include <unistd.h>

void log_putln(int fd) {
	write(fd, "\n", 1);
}
