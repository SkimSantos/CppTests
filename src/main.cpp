#include <iostream>

void testFunction(bool pressed) {
    std::cout << "Key test Function Pressed Test " << pressed << std::endl; 
}

void testButtonFunction(bool pressed) {
    std::cout << "Button Key Test " << pressed << std::endl;
}

#ifdef __linux__

#include "linux/window.h"
int main() {
    MyWindow window(1000, 600, "Testing Window");
    window.addCallToKey(38 , testFunction);
    window.addCallToButton(1, testButtonFunction);
    window.enableInput(true);
    window.run();
    window.removeCallFromKey(38, testFunction);
    window.removeCallFromButton(1, testButtonFunction);
    return 0;
}

#endif

#ifdef _WIN32

#endif