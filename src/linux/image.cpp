#define STB_IMAGE_IMPLEMENTATION
#include "linux/image.h"
#include "stb_image.h"
#include <iostream>

void Image::drawImage(Display *display, Window window, GC gc, const std::string &filepath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

    if(!data) {
        std::cerr << "Error: Failed to load image " << filepath << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    Visual *visual = DefaultVisual(display, screen);
    int depth = DefaultDepth(display, screen);

    XImage *image = XCreateImage(display, visual, depth, ZPixmap, 0, reinterpret_cast<char*>(data), width, height, 32, 0);

    if(!image) {
        std::cerr << "Error: Failed to create XImage" << std::endl;
        return;
    }

    // Draw Image
    XPutImage(display, window, gc, image, 0, 0, 50, 50, width, height);
    XFlush(display);

    // Cleanup
    
    stbi_image_free(data);
}
