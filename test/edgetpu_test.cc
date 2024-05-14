#include "object_detection.h"
#include "coral/test_utils.h"
#include <iostream>

int main()
{
    std::string path = "/home/rpi_Hee/Desktop/DigitalTwin/model/";
    std::string model = path + "yolov5s-int8-224_edgetpu.tflite";
    std::string label = path + "label.txt";
    std::string image = path + "cat.bmp";

    edgeTPU_USB acc = edgeTPU_USB(model, label);
    auto input = coral::GetInputFromImage(image,{640, 640, 3});
    acc.do_inference(input);

    auto outputs = acc.get_output();
    for(auto output : outputs)
        std::cout << output << "\n";
        
    std::cout << "done";
}