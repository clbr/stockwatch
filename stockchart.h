#ifndef STOCKCHART_H
#define STOCKCHART_H

#include <FL/Fl_Widget.H>

class stockchart: public Fl_Widget {

public:
	stockchart(int x, int y, int w, int h, const char label[]):
		Fl_Widget(x, y, w, h, label), src(NULL), movable(0) {}

	void draw();
	int handle(int e);

	void setsource(const std::vector<stockval> * const vec, const float tgt);

private:
	float min, max, target;
	const std::vector<stockval> *src;

	u32 dx, dy, dw, dh;
	u8 movable;
};

#endif
