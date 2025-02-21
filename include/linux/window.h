#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>

class MyWindow {
public:
    MyWindow(int width, int height, const std::string& title);
    ~MyWindow();

    void run();  // Main event loop
    void drawGraph();  // Placeholder for graph drawing
    void loadImage(const std::string& filepath);  // Load image

private:
    Display *display;
    Window window;
    GC gc;  // Graphics context
    int screen;
};

#endif
