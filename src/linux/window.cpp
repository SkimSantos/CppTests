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
    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | FocusChangeMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);
    gc = XCreateGC(display, window, 0, NULL);

    // Display the window
    XMapWindow(display, window);
}

MyWindow::~MyWindow() {
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    display = nullptr;
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
            if(focus_on) {
                if(keyFuncs[event.xkey.keycode] != nullptr) {
                    keyFuncs[event.xkey.keycode](true);
                }
            }
        } else if(event.type == KeyRelease) {
            if(focus_on) {
                if(keyFuncs[event.xkey.keycode] != nullptr) {
                    keyFuncs[event.xkey.keycode](false);
                }
            }
        } else if(event.type == ButtonPress) {
            if(focus_on) {
                if(buttonFuncs[event.xbutton.button] != nullptr) {
                    buttonFuncs[event.xbutton.button](true);
                }
            }
        } else if(event.type == ButtonRelease) {
            if(focus_on) {
                if(buttonFuncs[event.xbutton.button] != nullptr) {
                    buttonFuncs[event.xbutton.button](false);
                }
            }
        } else if(event.type == MotionNotify) {
            if(focus_on) {
                x_mouse_position = event.xmotion.x;
                y_mouse_position = event.xmotion.y;
            }
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

void MyWindow::addCallToKey(int keycode, void (*func)(bool)) {
    if(keycode > sizeof(keyFuncs)) {
        std::cout << "Keycode Out Of Size in addCallToKey. keycode : " << keycode << std::endl;
    }
    if(keyFuncs[keycode] != func) {
        keyFuncs[keycode] = func;
    }
}

void MyWindow::removeCallFromKey(int keycode, void (*func)(bool)) {
    if(keycode > sizeof(keyFuncs)) {
        std::cout << "Keycode Out Of Size in removeCallFromKey. keycode : " << keycode << std::endl;
    }
    if(keyFuncs[keycode] == func) {
        keyFuncs[keycode] = nullptr;
    }
}

void MyWindow::addCallToButton(int button, void (*func)(bool)) {
    if(button > sizeof(buttonFuncs)) {
        std::cout << "Buttom Index Out Of Size in addCallToButton. button : " << button << std::endl;
    }
    if(buttonFuncs[button] != func) {
        buttonFuncs[button] = func;
    }
}

void MyWindow::removeCallFromButton(int button, void (*func)(bool)) {
    if(button > sizeof(buttonFuncs)) {
        std::cout << "Buttom Index Out Of Size in removeCallFromButton. button : " << button << std::endl;
    }
    if(buttonFuncs[button] == func) {
        buttonFuncs[button] = nullptr;
    }
}

std::vector<int> MyWindow::getMousePosition() {
    return {x_mouse_position, y_mouse_position};
}

bool MyWindow::getIsFocus() {
    return focus_on;
}