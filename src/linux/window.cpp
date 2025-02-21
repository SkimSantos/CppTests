#include "linux/window.h"
#include "linux/image.h"
#include <iostream>

MyWindow::MyWindow(int width, int height, const std::string& title){
    display = XOpenDisplay(NULL);
    if(!display) {
        std::cerr << "Cannot Open X Display" << std::endl;
        exit(1);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, width, height, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    XStoreName(display, window, title.c_str());

    // Select input events (close window, key press, etc.)
    XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);
    gc = XCreateGC(display, window, 0, NULL);

    // Display the window
    XMapWindow(display, window);
}

MyWindow::~MyWindow() {
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void MyWindow::drawGraph() {

}

void MyWindow::loadImage(const std::string &filepath) {
    Image::drawImage(display, window, gc, filepath);
}

void MyWindow::run() {
    XEvent event;
    while(true) {
        XNextEvent(display, &event);
        if(event.type == KeyPress) {
            std::cout << "Key Pressed : " << event.xkey.keycode << std::endl;
        } else if(event.type == Expose) {
            drawGraph();
            loadImage("../images/warning.png");
        } else if (event.type == DestroyNotify) {
            break;
        }
    }
}