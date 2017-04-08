#ifndef DATA_H
#define DATA_H

#include <vector>

struct stockval {
	float val;
	time_t date;
};

struct stock {
	char ticker[16];
	float target;
	char comment[256];

	std::vector<stockval> weekly;
	std::vector<stockval> daily;
};

extern std::vector<stock> stocks;

#endif
