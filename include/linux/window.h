#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <string>

class MyWindow {
private:
    int x_mouse_position = 0;
    int y_mouse_position = 0;
    bool focus_on = false;
    Display *display;
    Window window;
    GC gc;  // Graphics context
    int screen;

public:
    MyWindow(int width, int height, const std::string& title);
    ~MyWindow();

    void run();  // Main event loop
    void drawGraph();  // Placeholder for graph drawing
    void loadImage(const std::string& filepath);  // Load image

    void changeInputHandle(long inputMask);
    void changeColor();
    void changeWindowSize(int width, int height);

    std::vector<int> getMousePosition();
    bool getIsFocus();

private:
    
};

#endif
