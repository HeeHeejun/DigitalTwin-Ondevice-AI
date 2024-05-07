
//g++ -o cam camera_test.cc `pkg-config --cflags --libs opencv4`
#include "camera.h"
#include <iostream>
int main()
{
    camera capture = camera();
    capture.do_capture();

    std::cout << "done\n";
    return 0;
}