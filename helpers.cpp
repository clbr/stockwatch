#include "main.h"

void die(const char fmt[], ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

void nukenewline(char buf[]) {
	u32 i;
	for (i = 0; buf[i]; i++) {
		if (buf[i] == '\n') {
			buf[i] = '\0';
			break;
		}
	}
}
