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
    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | FocusChangeMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
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
            std::cout << "Expose Event" << std::endl;
        } else if(event.type == KeyRelease) {

        } else if(event.type == ButtonPress) {
            
        } else if(event.type == ButtonRelease) {
            
        } else if(event.type == MotionNotify) {
            x_mouse_position = event.xmotion.x;
            y_mouse_position = event.xmotion.y;
        } else if(event.type == FocusIn) {
            focus_on = true;
        } else if(event.type == FocusOut) {
            focus_on = false;
        } else if (event.type == DestroyNotify) {
            break;
        }
    }
}

void MyWindow::changeInputHandle(long inputMask) {
    XSelectInput(display, window, inputMask);
}

void MyWindow::changeWindowSize(int width, int height) {

}

void MyWindow::changeColor() {
    
}

std::vector<int> MyWindow::getMousePosition() {
    return {x_mouse_position, y_mouse_position};
}

bool MyWindow::getIsFocus() {
    return focus_on;
}