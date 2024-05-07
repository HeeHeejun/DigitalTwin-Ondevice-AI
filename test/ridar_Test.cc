//g++ -o cam camera_test.cc `pkg-config --cflags --libs opencv4`
#include "ridar.h"
#include <iostream>
int main()
{
    ridar r = ridar();
    r.scan_data();

    std::cout << "done\n";

    return 0;
}