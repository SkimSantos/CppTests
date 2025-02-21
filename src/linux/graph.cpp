#include "linux/graph.h"

void Graph::drawLine(Display* display, Window win, GC gc, int x1, int y1, int x2, int y2) {
    XDrawLine(display, win, gc, x1, y1, x2, y2);
}

void Graph::drawCircle(Display* display, Window win, GC gc, int x, int y, int radius) {
    XDrawArc(display, win, gc, x - radius, y - radius, radius * 2, radius * 2, 0, 360 * 64);
}
