//fltk_exmpl.cpp
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

int main(int argc, char **argv)
{
    Fl_Window *win = new Fl_Window(300, 100, "hello");
    Fl_Box *b = new Fl_Box(0, 0, 300, 100, "Hello, World!");
    b->labelsize(36);
    win->end();
    win->show();
    return Fl::run();
}
