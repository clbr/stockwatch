#ifndef STOCKCHART_H
#define STOCKCHART_H

#include <FL/Fl_Widget.H>

class stockchart: public Fl_Widget {

public:
	stockchart(int x, int y, int w, int h, const char label[]):
		Fl_Widget(x, y, w, h, label), src(NULL) {}

	void draw();

	void setsource(std::vector<stockval> *vec, const float tgt);

private:
	float min, max, target;
	std::vector<stockval> *src;
};

#endif
