#include "linux/image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
#include <iostream>

void Image::drawImage(Display *display, Window window, GC gc, const std::string &filepath, XRectangle rect) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

    if(!data) {
        std::cerr << "Error: Failed to load image " << filepath << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    Visual *visual = DefaultVisual(display, screen);
    int depth = DefaultDepth(display, screen);

    XImage *image = XCreateImage(display, visual, 32, ZPixmap, 0, reinterpret_cast<char*>(data), rect.width, rect.height, 32, 0);

    if(!image) {
        std::cerr << "Error: Failed to create XImage" << std::endl;
        return;
    }

    // Draw Image
    XPutImage(display, window, gc, image, 0, 0, rect.x, rect.y, rect.width, rect.height);
    XFlush(display);

    // Cleanup
    
    stbi_image_free(data);
}

unsigned char* Image::getImageData(const std::string &filepath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

    if(!data) {
        std::cerr << "Error: Failed to load image " << filepath << std::endl;
    }

    return data;
}

unsigned char* Image::getScaleImagedata(const std::string &filepath, int new_width, int new_height) {
    int orig_width, orig_height, channels;
    unsigned char* input = stbi_load(filepath.c_str(), &orig_width, &orig_height, &channels, STBIR_RGBA); // force RGBA
    if (!input) {
        std::cerr << "Error: Failed to load image " << filepath << std::endl;
        return nullptr;
    }

    // Create output buffer
    unsigned char* output = new unsigned char[new_width * new_height * STBIR_RGBA];

    // Resize
    unsigned char* result = stbir_resize_uint8_linear(
        input, orig_width, orig_height, 0,
        output, new_width, new_height, 0,
        STBIR_RGBA // RGBA channels
    );

    stbi_image_free(input);

    if (!result) {
        fprintf(stderr, "Failed to resize image.\n");
        delete[] output;
        return nullptr;
    }

    return output;
}

void Image::freeImage(XImage* image) {
    stbi_image_free(image);
}
