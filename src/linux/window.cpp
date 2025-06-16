#include "linux/window.h"
#include "linux/image.h"
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/Xresource.h>
#include <iostream>

// Helper to find ARGB visual
Visual* MyWindow::get_argb_visual(Display* dpy, int screen, Colormap* out_colormap) {
    XVisualInfo vinfo;
    if (!XMatchVisualInfo(dpy, screen, 32, TrueColor, &vinfo)) {
        return nullptr;
    }

    *out_colormap = XCreateColormap(dpy, RootWindow(dpy, screen), vinfo.visual, AllocNone);
    return vinfo.visual;
}

MyWindow::MyWindow(int width, int height, const std::string& title){
    std::cout << "Create window" << std::endl;
    display = XOpenDisplay(nullptr);
    if(!display) {
        std::cerr << "Cannot Open X Display" << std::endl;
        exit(1);
    }
    
    screen = DefaultScreen(display);

    Colormap colormap;
    Visual* visual = get_argb_visual(display, screen, &colormap);
    if (!visual) {
        std::cerr << "No ARGB visual found\n" << std::endl;
        exit(1);
    }

    windowWidth = width;
    windowHeight = height;

    // Window attributes
    XSetWindowAttributes attrs;
    attrs.override_redirect = true;
    attrs.colormap = colormap;
    attrs.background_pixel = 0x000000EE;  // Fully transparent
    attrs.border_pixel = 0;
    attrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | FocusChangeMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;

    window = XCreateWindow(
        display, RootWindow(display, screen), 
        0, 0, width, height, 
        0, //border
        32, //depth
        InputOutput,
        visual,
        CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel | CWEventMask,
        &attrs
    );

    //XStoreName(display, window, title.c_str());

    // Select input events (close window, key press, etc.)
    XSelectInput(display, window, attrs.event_mask);
    gc = XCreateGC(display, window, 0, nullptr);

    // Display the window
    XMapWindow(display, window);

    // Let window stay on top
    XRaiseWindow(display, window);

    // Enable input shape extension
    int shape_event_base, shape_error_base;
    if (!XShapeQueryExtension(display, &shape_event_base, &shape_error_base)) {
        exit(1);
    }

    XFlush(display);
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
        switch (event.type)
        {
        case KeyPress:
            if(focus_on) {
                if(keyFuncs[event.xkey.keycode] != nullptr) {
                    keyFuncs[event.xkey.keycode](true);
                }
            }
            break;
        case KeyRelease:
            if(focus_on) {
                if(keyFuncs[event.xkey.keycode] != nullptr) {
                    keyFuncs[event.xkey.keycode](false);
                }
            }
            break;

        case ButtonPress:
            if(focus_on) {
                if(buttonFuncs[event.xbutton.button] != nullptr) {
                    buttonFuncs[event.xbutton.button](true);
                }
            }
            break;
        
        case ButtonRelease:
            if(focus_on) {
                if(buttonFuncs[event.xbutton.button] != nullptr) {
                    buttonFuncs[event.xbutton.button](false);
                }
            }
            break;
        
        case MotionNotify:
            std::cout << "Mouse Motion" << std::endl;
            if(focus_on) {
                x_mouse_position = event.xmotion.x;
                y_mouse_position = event.xmotion.y;
            }
        
        case FocusIn:
            std::cout << "Focus" << std::endl;
            focus_on = true;
            break;
        case FocusOut:
            focus_on = false;
            break;

        default:
            break;
        }
    }
}

void MyWindow::enableInput(bool enable) {
    XRectangle rect = {0, 0, (unsigned short)windowWidth, (unsigned short)windowHeight};

    if(enable) {
        XShapeCombineRectangles(display, window, ShapeInput, 0, 0, &rect, 1, ShapeSet, 0);
    } else {
        XShapeCombineRectangles(display, window, ShapeInput, 0, 0, nullptr, 0, ShapeSet, 0);
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