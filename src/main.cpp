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
        rect.x = currentWindow->getScreenWidth() - 100;
        rect.y = currentWindow->getScreenHeight() - 100;
        rect.width = 100;
        rect.height = 100;
        currentWindow->setInputButton( currentWindow->createButton(rect, "app_launcher.png", onExitPress));

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