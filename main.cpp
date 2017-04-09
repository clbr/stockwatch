#include "main.h"

std::vector<stock> stocks;

class clickpack: public Fl_Pack {
public:
	clickpack(int x, int y, int w, int h): Fl_Pack(x, y, w, h) {}

	int handle(int e) {
		if (e == FL_PUSH) {
			do_callback();
			return 1;
		}

		return Fl_Pack::handle(e);
	}
};

Fl_Scroll *scroll = (Fl_Scroll *) 0;
Fl_Pack *list = (Fl_Pack *) 0;
Fl_Box *status = (Fl_Box *) 0;

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

static void picked(Fl_Widget *, void *data) {

	const u32 num = (u32) (uintptr_t) data;

	printf("clicked %u\n", num);
}

static void load() {

	status->label("Loading...");

	const char * const home = getenv("HOME");
	if (!home)
		die("No home?\n");
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "%s/.stockwatch", home);

	FILE *f = fopen(path, "r");
	if (!f) die("Can't open config file\n");

	while (fgets(path, PATH_MAX, f)) {
		nukenewline(path);

		stock s;
		if (sscanf(path, "%s %f", s.ticker, &s.target) != 2)
			die("Malformed line %s\n", path);

		const char *ptr = strchr(path, ' ');
		ptr = strchr(ptr + 1, ' ');
		ptr++;

		strncpy(s.comment, ptr, sizeof(s.comment));
		s.comment[sizeof(s.comment) - 1] = '\0';

		stocks.push_back(s);
	}

	fclose(f);

	// Fetch data TODO
	// Sort TODO

	u32 i;
	const u32 max = stocks.size();
	for (i = 0; i < max; i++) {
		Fl_Pack *p = new clickpack(0, 0, 180, 20);
		p->type(Fl_Pack::HORIZONTAL);
		p->box(FL_SHADOW_FRAME);

		Fl_Box *b = new Fl_Box(0, 0, 70, 20, stocks[i].ticker);
		b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		b->labelfont(FL_HELVETICA | FL_BOLD);
		//b->box(FL_UP_BOX);

		char buf[32];
		sprintf(buf, "%.2f", stocks[i].target);

		b = new Fl_Box(0, 0, 55, 20);
		b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		//b->labelfont(FL_HELVETICA | FL_BOLD);
		b->copy_label(buf);
		//b->box(FL_UP_BOX);

		// TODO last day
		const float val = rand() % 2 ? 4.45 : -4.45;
		sprintf(buf, "%.2f%%", val);

		b = new Fl_Box(0, 0, 55, 20);
		b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		b->labelcolor(val < 0 ? FL_DARK_RED : FL_DARK_GREEN);
		b->labelfont(FL_HELVETICA | FL_BOLD);
		b->copy_label(buf);
		//b->box(FL_UP_BOX);

		p->tooltip(stocks[i].comment);
		p->callback(picked);
		p->user_data((void *) i);

		p->end();
		list->add(p);
	}

	status->label("");
}

int main(int argc, char **argv) {

	Fl::scheme("gtk+");

	Fl_Double_Window *w;
	{
		Fl_Double_Window *o =
		    new Fl_Double_Window(1105, 855, "StockWatch");
		w = o;
		{
			scroll = new Fl_Scroll(0, 0, 210, 855);
			scroll->type(Fl_Scroll::VERTICAL_ALWAYS);
			scroll->box(FL_UP_BOX);
			{
				list = new Fl_Pack(0, 0, 200, 825);
				list->end();
				Fl_Group::current()->resizable(list);
			}	// Fl_Pack* list
			scroll->end();
		}		// Fl_Scroll* scroll
		{
			Fl_Chart *o = new Fl_Chart(215, 4, 885, 417);
			o->box(FL_NO_BOX);
			o->color((Fl_Color) FL_BACKGROUND_COLOR);
			o->selection_color((Fl_Color) FL_BACKGROUND_COLOR);
			o->labeltype(FL_NORMAL_LABEL);
			o->labelfont(0);
			o->labelsize(14);
			o->labelcolor((Fl_Color) FL_FOREGROUND_COLOR);
			o->align(FL_ALIGN_CENTER);
			o->when(FL_WHEN_RELEASE);
		}		// Fl_Chart* o
		{
			status = new Fl_Box(535, 599, 215, 92);
			Fl_Group::current()->resizable(status);
		}		// Fl_Box* status
		o->size_range(1105, 855);
		o->end();
	}			// Fl_Double_Window* o
	w->show(argc, argv);

	load();

	return Fl::run();
}
