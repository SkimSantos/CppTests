#include <iostream>
#include "linux/window.h"

int main() {
    MyWindow window(800, 600, "Testing Window");
    window.run();
    return 0;
}