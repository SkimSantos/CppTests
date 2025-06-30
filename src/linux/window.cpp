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

MyWindow::MyWindow(const std::string& title, void (*callback)()){
    exposeCallback = callback;
    std::cout << "Create window" << std::endl;
    display = XOpenDisplay(nullptr);
    if(!display) {
        std::cerr << "Cannot Open X Display" << std::endl;
        exit(1);
    }
    
    screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    Colormap colormap;
    Visual* visual = get_argb_visual(display, screen, &colormap);
    if (!visual) {
        std::cerr << "No ARGB visual found\n" << std::endl;
        exit(1);
    }

    windowWidth = DisplayWidth(display, screen);
    windowHeight = DisplayHeight(display, screen);

    // Window attributes
    XSetWindowAttributes attrs;
    attrs.override_redirect = true;
    attrs.colormap = colormap;
    attrs.background_pixel = 0x00000000;  // Fully transparent
    attrs.border_pixel = 0;
    attrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | FocusChangeMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

    window = XCreateWindow(
        display, root, 
        0, 0, windowWidth, windowHeight, 
        0, //border
        32, //depth
        InputOutput,
        visual,
        CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel | CWEventMask,
        &attrs
    );

    // Display the window
    XMapWindow(display, window);
    
    // Let window stay on top
    XRaiseWindow(display, window);
    XFlush(display);
    
    gc = XCreateGC(display, window, 0, nullptr);
    XSetInputFocus(display, screen, RevertToPointerRoot, CurrentTime);
    
    // Enable input shape extension
    int shape_event_base, shape_error_base;
    if (!XShapeQueryExtension(display, &shape_event_base, &shape_error_base)) {
        exit(1);
    }

    enableInput(true);
}

MyWindow::~MyWindow() {
    for(int i = 0; i < buttons.size(); i++) {
        buttons[i].ximage;
        Image::freeImage(buttons[i].ximage);
    }
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    display = nullptr;
}

void MyWindow::drawGraph() {

}

void MyWindow::loadImage(const std::string &filepath, XRectangle rect) {
    Image::drawImage(display, window, gc, filepath, rect);
}

void MyWindow::run() {
    XEvent event;
    while(true) {
        XNextEvent(display, &event);
        switch (event.type)
        {
        case KeyPress:
            if(inputActive) {
                if(keyFuncs[event.xkey.keycode] != nullptr) {
                    keyFuncs[event.xkey.keycode](true);
                }
            }
            break;
        case KeyRelease:
            if(inputActive) {
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
                bool calledButton = false;
                for(int i = 0; i < buttons.size(); i++) {
                    if(buttons[i].x <= x_mouse_position && buttons[i].y <= y_mouse_position) {
                        if((buttons[i].x + buttons[i].width) >= x_mouse_position && (buttons[i].y + buttons[i].height) >= y_mouse_position) {
                            calledButton = true;
                            if(buttons[i].callback != nullptr) {
                                buttons[i].callback();
                            }
                        }
                    }
                }
                if(event.xbutton.button == 3) {
                    enableInput(!inputActive);
                }
                else if(buttonFuncs[event.xbutton.button] != nullptr && !calledButton) {
                    buttonFuncs[event.xbutton.button](false);
                }
            }
            break;
        
        case MotionNotify:
            if(focus_on) {
                x_mouse_position = event.xmotion.x;
                y_mouse_position = event.xmotion.y;
            }
        
        case FocusIn:
            focus_on = true;
            break;
        case FocusOut:
            focus_on = false;
            break;
        
        case Expose:
            if(exposeCallback != nullptr) {
                exposeCallback();
            }
            break;

        default:
            break;
        }
    }
}

void MyWindow::enableInput(bool enable) {
    inputActive = enable;
    
    if(enable) {
        XRectangle rect = {0, 0, (unsigned short)windowWidth, (unsigned short)windowHeight};
        XShapeCombineRectangles(display, window, ShapeInput, 0, 0, &rect, 1, ShapeSet, 0);
        XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    } else {
        XRectangle rect = {0, 0, (unsigned short)0, (unsigned short)0};
        if(inputButton != nullptr) {
            rect.x = inputButton->x;
            rect.y = inputButton->y;
            rect.width = inputButton->width;
            rect.height = inputButton->height;
        }
        XShapeCombineRectangles(display, window, ShapeInput, 0, 0, &rect, 1, ShapeSet, 0);
        XUngrabKeyboard(display, CurrentTime);
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

MyWindow::Button *MyWindow::createButton(XRectangle rect, const std::string& filepath, void (*call)()) {
    Button tempButton;
    tempButton.x = rect.x;
    tempButton.y = rect.y;
    tempButton.width = rect.width;
    tempButton.height = rect.height;
    tempButton.callback = call;

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo)) {
        fprintf(stderr, "No ARGB visual\n");
        return nullptr;
    }

    tempButton.pixels = Image::getScaleImagedata(filepath, rect.width, rect.height);
    tempButton.ximage = XCreateImage(
        display, vinfo.visual, 32, ZPixmap, 0,
        (char*)tempButton.pixels,
        tempButton.width, tempButton.height,
        32, tempButton.width * 4
    );

    buttons.push_back(tempButton);

    XPutImage(display, window, gc, tempButton.ximage, 0, 0, tempButton.x, tempButton.y,
        tempButton.width, tempButton.height);
    
    return &buttons.back();
}

void MyWindow::setInputButton(Button *bt) {
    inputButton = bt;
}

std::vector<int> MyWindow::getMousePosition() {
    return {x_mouse_position, y_mouse_position};
}

bool MyWindow::getIsFocus() {
    return focus_on;
}