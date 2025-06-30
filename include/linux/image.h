#ifndef IMAGE_H
#define IMAGE_H

#include <X11/Xlib.h>
#include <string>

class Image {
public:
    static void drawImage(Display* display, Window win, GC gc, const std::string& filepath, XRectangle rect);
    static unsigned char* getImageData(const std::string &filepath);
    static unsigned char* getScaleImagedata(const std::string &filepath, int new_width, int new_height);
    static void freeImage(XImage* image);
};

#endif