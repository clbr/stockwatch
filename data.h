#ifndef DATA_H
#define DATA_H

#include <vector>

struct stock {
	char ticker[16];
	float target;
	char comment[256];
};

extern std::vector<stock> stocks;

#endif
