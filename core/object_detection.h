#ifndef DETECT_H
#define DETECT_H

#include <fstream>

#include "coral/detection/adapter.h"
#include "coral/tflite_utils.h"
#include "tensorflow/lite/interpreter.h"


//ymin,xmin,ymax,xmax
struct Box
{
    float ymin, xmin, ymax, xmax;
    Box(float _ymin, float _xmin, float _ymax, float _xmax) : ymin(_ymin), xmin(_xmin), ymax(_ymax), xmax(_xmax){}
};
struct Object 
{
    int id;
    std::string label;
    Box box;
    float distance;
    Object(int _id, std::string _label, Box _box) : id(_id), label(_label), box(_box) {}
};

class edgeTPU_USB
{
    std::unique_ptr<tflite::FlatBufferModel> model;
    std::shared_ptr<edgetpu::EdgeTpuContext> edgetpu_context;
    std::unique_ptr<tflite::Interpreter> interpreter;
    std::unordered_map<int, std::string> labels;
    size_t num_devices;
    const float score_threshold = 0.7;

    edgeTPU_USB(){}
    edgeTPU_USB(const std::string& model_path, const std::string& labels);

    void do_inference(std::vector<uint8_t>& input_img);
    std::vector<Object>& get_output();
};

#endif