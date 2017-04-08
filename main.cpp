#include "ui.h"
#include "data.h"

std::vector<stock> stocks;

Fl_Scroll *scroll = (Fl_Scroll *) 0;
Fl_Pack *list = (Fl_Pack *) 0;
Fl_Box *status = (Fl_Box *) 0;

int main(int argc, char **argv) {

	Fl_Double_Window *w;
	{
		Fl_Double_Window *o =
		    new Fl_Double_Window(1105, 855, "StockWatch");
		w = o;
		{
			scroll = new Fl_Scroll(0, 0, 210, 855);
			scroll->type(2);
			scroll->box(FL_UP_BOX);
			{
				list = new Fl_Pack(0, 0, 180, 825);
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
	return Fl::run();
}
