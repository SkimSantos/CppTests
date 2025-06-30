#include <iostream>

#ifdef __linux__

#include "linux/window.h"
#include "linux/image.h"

MyWindow* currentWindow;

void testFunction(bool pressed) {
    std::cout << "Key test Function Pressed Test " << pressed << std::endl; 
}

void testButtonFunction(bool pressed) {
    std::cout << "Button Key Test " << pressed << std::endl;
}

void onExitPress() {
    std::cout << "On Exit" << std::endl;
    if(currentWindow != nullptr) {
        currentWindow->removeCallFromKey(38, testFunction);
        currentWindow->removeCallFromButton(1, testButtonFunction);
    }
    exit(1);
}

void onWindowExpose() {
    if(currentWindow != nullptr) {
        XRectangle rect;
        rect.x = 100;
        rect.y = 20;
        rect.width = 50;
        rect.height = 50;
        currentWindow->setInputButton( currentWindow->createButton(rect, "warning.png", onExitPress));

        currentWindow->addCallToKey(38 , testFunction);
        currentWindow->addCallToButton(1, testButtonFunction);
    }
}

int main() {
    MyWindow window("Testing Window", onWindowExpose);
    currentWindow = &window;
    window.run();
    return 0;
}

#endif

#ifdef _WIN32

#endif