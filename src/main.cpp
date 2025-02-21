#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;

    display = XOpenDisplay(NULL);
    if(display == NULL) {
        std::cerr << "Cannot open X Display" << std::endl;
        return 1;
    }

    screen = DefaultScreen(display);

    // Create a simple window
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100, 800, 600, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    // Select input events (close window, key press, etc.)
    XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

    // Display the window
    XMapWindow(display, window);

    // Set window title
    XStoreName(display, window, "Testing Window");

    // Event loop
    while (true) {
        XNextEvent(display, &event);

        // Handle events
        if (event.type == Expose) {
            // Redraw window if needed
        } else if (event.type == KeyPress) {
            std::cout << "Key Pressed! Closing window..." << std::endl;
        } else if (event.type == DestroyNotify) {
            break;
        }
    }

    // Close display connection
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}