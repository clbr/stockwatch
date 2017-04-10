#include "main.h"

std::vector<stock> stocks;

static stockchart *daychart, *yearchart;

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

static Fl_Scroll *scroll = (Fl_Scroll *) 0;
static Fl_Pack *list = (Fl_Pack *) 0;
static Fl_Box *status = (Fl_Box *) 0;

static void picked(Fl_Widget *, void *data) {

	const u32 num = (u32) (uintptr_t) data;
	//printf("clicked %u\n", num);

	yearchart->setsource(&stocks[num].weekly, stocks[num].target);
	daychart->setsource(&stocks[num].daily, stocks[num].target);

	char buf[384];
	sprintf(buf, "%s\n\n%s\ntarget %.2f",
		stocks[num].ticker,
		stocks[num].comment,
		stocks[num].target);

	status->copy_label(buf);
}

static void import(FILE * const f, std::vector<stockval> &vec) {

	char buf[PATH_MAX];

	while (fgets(buf, PATH_MAX, f)) {
		if (buf[0] != '2')
			continue;

		nukenewline(buf);

		stockval sv;

		if (sscanf(buf, "%hu-%hhu-%hhu,%f", &sv.year, &sv.month, &sv.day, &sv.val) != 4) {
			printf("Couldn't import data line '%s'\n", buf);
			break;
		}

		vec.push_back(sv);
	}
}

static void fetch() {

	u32 i;
	const u32 max = stocks.size();

	const time_t now = time(NULL);
	struct tm dated, datedmonths, datedyears;
	localtime_r(&now, &dated);

	const u32 secs_per_day = 60 * 60 * 24;

	const time_t monthback = now - secs_per_day * 30;
	const time_t yearsback = now - secs_per_day * 365 * 5;
	localtime_r(&monthback, &datedmonths);
	localtime_r(&yearsback, &datedyears);

	char buf[PATH_MAX];

	scroll->hide();

	for (i = 0; i < max; i++) {
		Fl::check();

		sprintf(buf, "wget -q -O - 'http://chart.finance.yahoo.com/table.csv?s=%s&a=%u&b=%u&c=%u&d=%u&e=%u&f=%u&g=d&ignore=.csv'",
			stocks[i].ticker,
			datedmonths.tm_mon, datedmonths.tm_mday, datedmonths.tm_year + 1900,
			dated.tm_mon, dated.tm_mday, dated.tm_year + 1900);

//		FILE *f = popen("cat daily.sample", "r");
		FILE *f = popen(buf, "r");
		if (!f) die("Failed to fetch data\n");

		import(f, stocks[i].daily);

		pclose(f);

		// And weekly

		sprintf(buf, "wget -q -O - 'http://chart.finance.yahoo.com/table.csv?s=%s&a=%u&b=%u&c=%u&d=%u&e=%u&f=%u&g=w&ignore=.csv'",
			stocks[i].ticker,
			datedyears.tm_mon, datedyears.tm_mday, datedyears.tm_year + 1900,
			dated.tm_mon, dated.tm_mday, dated.tm_year + 1900);

//		f = popen("cat weekly.sample", "r");
		f = popen(buf, "r");
		if (!f) die("Failed to fetch data\n");

		import(f, stocks[i].weekly);

		pclose(f);
	}
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

	// Fetch data
	struct timeval before, after;
	gettimeofday(&before, NULL);
	fetch();
	gettimeofday(&after, NULL);

	timersub(&after, &before, &after);
	printf("Fetching data took %lu ms\n", after.tv_sec * 1000 + after.tv_usec / 1000);

	// Sort
	std::sort(stocks.begin(), stocks.end());

	u32 i;
	const u32 max = stocks.size();
	for (i = 0; i < max; i++) {
		if (stocks[i].daily.size() < 5) {
			fl_alert("Failed to fetch data for %s",
				stocks[i].ticker);
			continue;
		}

		Fl_Pack *p = new clickpack(0, 0, 200, 20);
		p->type(Fl_Pack::HORIZONTAL);
		p->box(FL_SHADOW_FRAME);

		Fl_Box *b = new Fl_Box(0, 0, 70, 20, stocks[i].ticker);
		b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
		b->labelfont(FL_HELVETICA | FL_BOLD);
		if (stocks[i].daily[0].val < stocks[i].target)
			b->labelcolor(FL_DARK_GREEN);
		//b->box(FL_UP_BOX);

		char buf[32];
		sprintf(buf, "%.2f", stocks[i].daily[0].val);

		b = new Fl_Box(0, 0, 55, 20);
		b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		//b->labelfont(FL_HELVETICA | FL_BOLD);
		b->copy_label(buf);
		//b->box(FL_UP_BOX);

		const float val = stocks[i].daily[0].val * 100 / stocks[i].daily[1].val - 100;
		sprintf(buf, "%.2f%%", val);

		b = new Fl_Box(0, 0, 60, 20);
		b->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		b->labelcolor(val < 0 ? FL_DARK_RED : FL_DARK_GREEN);
		b->labelfont(FL_HELVETICA | FL_BOLD);
		b->copy_label(buf);
		//b->box(FL_UP_BOX);

		char tipbuf[256 + 16];
		sprintf(tipbuf, "%s, target %.2f", stocks[i].comment, stocks[i].target);

		p->copy_tooltip(tipbuf);
		p->callback(picked);
		p->user_data((void *) i);

		p->end();
		list->add(p);
	}

	status->label("");
	scroll->show();
	picked(NULL, 0);
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
			Fl_Tabs *tabs = new Fl_Tabs(215, 4, 885, 417);

			yearchart = new stockchart(215, 38, 885, 383, "5 years");

			daychart = new stockchart(215, 38, 885, 383, "30 days");

			tabs->end();

			tabs->value(o);
		}		// Fl_Chart* o
		{
			status = new Fl_Box(535 - 100, 599, 215 + 200, 92);
			Fl_Group::current()->resizable(status);
		}		// Fl_Box* status
		o->size_range(1105, 855);
		o->end();
	}			// Fl_Double_Window* o
	w->show(argc, argv);

	load();

	return Fl::run();
}
