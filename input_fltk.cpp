/* input probe */

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Secret_Input.H>

static void say_callback(Fl_Widget *w, void *user)
{
	printf("%s\n", ((Fl_Secret_Input*)user)->value());
    ((Fl_Secret_Input*)user)->value("");
	((Fl_Secret_Input*)user)->take_focus();
}

static void clear_callback(Fl_Widget *w, void *user)
{
	((Fl_Secret_Input*)user)->value("");
	((Fl_Secret_Input*)user)->take_focus();
}

static void exit_callback(Fl_Widget *w, void *user)
{
    Fl_Widget *p;
    do {
        p = w->parent();
        if(p)
            w = p;
    } while(p);
    w->hide();
}

enum {
	spacing = 20,
	input_h = 30,
	label_w = 50,
	button_w = 100,
	button_h = 40
};

int main()
{
    int win_h = button_h * 2 + spacing * 3;
    int win_w = button_w * 3 + spacing * 4;
    Fl_Window *win = new Fl_Window(win_w, win_h, "Input demo");
	
    int inp_w = button_w * 3 + 2 * spacing - label_w;
	Fl_Secret_Input *inp = new Fl_Secret_Input(spacing + label_w, spacing,
	       			inp_w, input_h, "Type:");
    inp->callback(say_callback, (void *)inp);
    inp->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);

    int button_y = spacing * 2 + input_h;
    Fl_Button *say_b = 
            new Fl_Button(spacing, button_y, button_w, 
                    button_h, "Say!");
    say_b->callback(say_callback, (void *)inp);

    Fl_Button *clr_b = 
            new Fl_Button(spacing * 2 + button_w, button_y,
                    button_w, button_h, "Clear");
    clr_b->callback(clear_callback, (void *)inp);

    Fl_Button *exit_b = 
            new Fl_Button(spacing * 3 + button_w *2, button_y,
                    button_w, button_h, "Exit");
    exit_b->callback(exit_callback, 0);

    win->end();
    win->show();
    return Fl::run();
}
