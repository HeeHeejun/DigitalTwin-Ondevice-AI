#ifndef DETECT_H
#define DETECT_H

#include <fstream>

#include "coral/detection/adapter.h"
#include "coral/examples/file_utils"
#include "coral/tflite_utils.h"
#include "tensorflow/lite/interpreter.h"
#include "error.h"

test edgeTPU_test_level = TEST;

struct Box
{
    float ymin, xmin, ymax, xmax;
    Box(float _ymin, float _xmin, float _ymax, float _xmax) : ymin(_ymin), xmin(_xmin), ymax(_ymax), xmax(_xmax){}
};

std::string ToString(const Box& box) 
{
    return absl::Substitute("BBox(ymin=$0,xmin=$1,ymax=$2,xmax=$3)", box.ymin, box.xmin, box.ymax, box.xmax);
}

std::ostream& operator<<(std::ostream& stream, const Box& box) 
{
    return stream << ToString(box);
}
struct Object 
{
    int id;
    std::string label;
    Box box;
    float distance;
    Object(int _id, std::string _label, Box _box) : id(_id), label(_label), box(_box) {}
};

std::string ToString(const Object& obj) 
{
    return absl::Substitute("Object(id=$0,score=$1,bbox=$2)", obj.id, obj.label, obj.score, ToString(obj.box));
}

std::ostream& operator<<(std::ostream& stream, const Object& box) 
{
    return stream << ToString(box);
}

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