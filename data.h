#ifndef DATA_H
#define DATA_H

#include <vector>

struct stockval {
	float val;
	u16 year;
	u8 month;
	u8 day;
};

struct stock {
	char ticker[16];
	float target;
	char comment[256];

	std::vector<stockval> weekly;
	std::vector<stockval> daily;

	bool operator < (const stock &other) const {
		// TODO compare daily value
		return target < other.target;
	}
};

extern std::vector<stock> stocks;

#endif
