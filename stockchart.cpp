#include "main.h"
#include <FL/fl_draw.H>

void stockchart::draw() {

	u32 bx, by, bw, bh;
	bx = x() + 5;
	by = y() + 5;
	bw = w() - 10;
	bh = h() - 10;

	u32 dx, dy, dw, dh;
	dx = bx + 1;
	dy = by + 1;
	dw = bw - 2;
	dh = bh - 2;

	fl_color(FL_BLACK);
	fl_rectf(bx, by, bw, bh);

	fl_color(FL_WHITE);
	fl_rectf(dx, dy, dw, dh);

	if (!src)
		return;
}

void stockchart::setsource(const std::vector<stockval> * const vec, const float tgt) {
	src = vec;
	target = tgt;

	if (src) {
		// calculate min and max
		const u32 maxsize = src->size();
		u32 i;

		min = (*src)[0].val;
		max = (*src)[0].val;

		for (i = 1; i < maxsize; i++) {
			const float v = (*src)[i].val;
			if (v < min)
				min = v;
			if (v > max)
				max = v;
		}

		// Target
		if (target < min)
			min = target;
		if (target > max)
			max = target;

		// Expand
		const float area = (max - min) / 6;
		min -= area;
		max += area;

		if (min < 0)
			min = 0;
	}

	redraw();
}
