#include "../include/strlib.h"
#include "../include/format.h"
#include <unistd.h>
#include <stddef.h>

static int usage(char*argv0) {
	ulz_dprintf(2, "%s - dumps input from stdin as hex stream\n", argv0);
	return 1;
}

int main(int argc, char** argv) {
	unsigned char buf[1024];
	char out[2048+1];
	if(argc > 1) return usage(argv[0]);
	while(1) {
		ssize_t n = read(0, buf, sizeof buf);
		if(n <= 0) break;
		raw2hex(out, buf, n);
		ulz_printf("%s", out);
	}
	ulz_printf("\n");
	return 0;
}
