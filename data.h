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
		if (!daily.size() || !other.daily.size())
			return 0;

		const float mydiff = daily[0].val / target;
		const float otherdiff = other.daily[0].val / other.target;

		return mydiff < otherdiff;
	}
};

extern std::vector<stock> stocks;

#endif
