#include "main.h"
#include <FL/fl_draw.H>

void stockchart::draw() {

	u32 i;

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

	const u32 maxsize = src->size();

	dx += 48;
	dw -= 48;

	dh -= 24;

	fl_color(FL_BLACK);
	fl_rect(dx, dy - 1, dw + 1, dh);

	// Data area
	dx++;
	dy++;
	dw -= 2;
	dh -= 2;

	// Vertical day/month lines
	bool yearly = false;
	if ((*src)[0].year - (*src)[maxsize - 1].year > 1)
		yearly = true;

	char tmp[32];
	int tw, th;

	fl_font(FL_HELVETICA, 14);
	const float pointw = dw / (float) maxsize;
	if (yearly) {
		for (i = 1; i < maxsize; i++) {
			const stockval &cur = (*src)[i - 1];
			const stockval &prev = (*src)[i];

			if (cur.month != prev.month) {
				const u32 x = dx + (dw - i * pointw);

				if (cur.year != prev.year) {
					fl_color(FL_BLACK);

					sprintf(tmp, "%u", cur.year);
					fl_measure(tmp, tw, th, 0);
					fl_draw(tmp, x - tw / 2,
						dy + dh + 6 - fl_descent() + fl_height());

					fl_color(FL_GRAY0 + 9);
				} else {
					fl_color(FL_GRAY0 + 18);

					fl_font(FL_HELVETICA, 11);
					sprintf(tmp, "%u", cur.month);
					fl_measure(tmp, tw, th, 0);
					fl_draw(tmp, x - tw / 2,
						dy + dh + 1 - fl_descent() + fl_height());

					fl_font(FL_HELVETICA, 14);
				}

				// Draw line here
				fl_line(x, dy, x, dy + dh - 2);
			}
		}
	} else {
		fl_color(FL_GRAY0 + 18);
	}
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
