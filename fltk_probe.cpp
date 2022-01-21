/* fltk_probe.cpp */
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

struct controls {
    Fl_Radio_Round_Button *rb[3];
    Fl_Check_Button *cb;
    Fl_Box *box;
};

static const int colors[] = { FL_RED, FL_GREEN, FL_BLUE };
static const char *const colnames[] = { "red", "green", "blue" };

static void set_callback(Fl_Widget *w, void *user)
{
    controls *c = (controls*)user;
    int i;
    for(i = 0; i < 3; i++) {
        if(c->rb[i]->value()) {
            c->box->color(colors[i]);
            break;
        }
    }
    c->box->label(c->cb->value() ? "A" : "");
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
    spacing = 2,
    button_w = 150,
    button_h = 40,
    option_w = 120,
    option_h = 20,
    letter_size = 90
};

main(int argc, char **argv)
{
    int win_w = button_w * 2 + spacing * 3;
    int win_h = option_h * 4 + spacing * 7;
    Fl_Window *win = new Fl_Window(win_w, win_h, "buttons demo");
    controls *ctrl = new controls;

    int i;
    for(i = 0; i < 3; i++) {
        int y = spacing * (i+1) + option_h * i;
        ctrl->rb[i] = new Fl_Radio_Round_Button(spacing, y, 
                                        option_w, option_h, colnames[i]);
    }
    ctrl->cb = 
        new Fl_Check_Button(spacing, 5*spacing + 3*option_h,
                                        option_w, option_h, "show letter");
    int box_x = 2 * spacing + option_w;
    int box_w = 2 * button_w + spacing - option_w - spacing;
    int box_h = 4 * option_h + 4 * spacing;
    ctrl->box = new Fl_Box(box_x, spacing, box_w, box_h);
    ctrl->box->labelsize(letter_size);
    ctrl->box->labelcolor(FL_WHITE);
    ctrl->box->box(FL_FLAT_BOX);
    
    int buttons_y = 6 * spacing + 4 * option_h;
    
    Fl_Button *set_b = 
        new Fl_Button(spacing, buttons_y, button_w, button_h, "Set!");
    set_b->callback(set_callback, (void*)ctrl);

    Fl_Button *close_b = 
        new Fl_Button(2*spacing + button_w, buttons_y, button_w,
                                    button_h, "Quit");
    close_b->callback(exit_callback, 0);
    win->end();
    win->show();
    return Fl::run();
}
