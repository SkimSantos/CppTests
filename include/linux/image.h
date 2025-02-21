#ifndef IMAGE_H
#define IMAGE_H

#include <X11/Xlib.h>
#include <string>

class Image {
public:
    static void drawImage(Display* display, Window win, GC gc, const std::string& filepath);
};

#endif