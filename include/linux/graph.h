#ifndef GRAPH_H
#define GRAPH_H

#include <X11/Xlib.h>

class Graph {
public:
    static void drawLine(Display* display, Window win, GC gc, int x1, int y1, int x2, int y2);
    static void drawCircle(Display* display, Window win, GC gc, int x, int y, int radius);
};

#endif
