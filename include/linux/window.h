#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <string>

class MyWindow {
public:
    struct Button
    {
        int x, y, width, height;
        unsigned char* pixels = nullptr;
        XImage* ximage = nullptr;
        void (*callback)();
    };
    
private:
    int x_mouse_position = 0;
    int y_mouse_position = 0;
    int windowWidth = 0;
    int windowHeight = 0;
    bool focus_on = false;
    Display *display;
    Window window;
    GC gc;  // Graphics context
    int screen;

    bool inputActive = true;

    bool created = false;

    std::vector<Button> buttons;
    Button *inputButton;

    void (*keyFuncs[255])(bool) = {nullptr};
    void (*buttonFuncs[5])(bool) = {nullptr};

    void (*exposeCallback)() = nullptr;

public:
    MyWindow(const std::string& title, void (*func)());
    ~MyWindow();

    void run();  // Main event loop
    void drawGraph();  // Placeholder for graph drawing
    void loadImage(const std::string& filepath, XRectangle rect);  // Load image

    void changeInputHandle(long inputMask);
    void changeColor();
    void changeWindowSize(int width, int height);

    void enableInput(bool enable);

    void addCallToKey(int keycode, void (*func)(bool));
    void removeCallFromKey(int keycode, void (*func)(bool));

    void addCallToButton(int button, void (*func)(bool));
    void removeCallFromButton(int button, void (*func)(bool));

    Button *createButton(XRectangle rect, const std::string& filepath, void (*call)());
    void setInputButton(Button *bt);

    std::vector<int> getMousePosition();
    bool getIsFocus();

private:
    Visual* get_argb_visual(Display* dpy, int screen, Colormap* out_colormap);
    
};

#endif
