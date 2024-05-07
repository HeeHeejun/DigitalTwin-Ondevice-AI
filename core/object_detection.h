#ifndef DETECT_H
#define DETECT_H

#include <fstream>

#include "coral/classification/adapter.h"
#include "coral/examples/file_utils.h"
#include "coral/tflite_utils.h"
#include "tensorflow/lite/interpreter.h"

class edgeTPU_USB
{
    const std::unique_ptr<tflite::FlatBufferModel> model;
    std::shared_ptr<edgetpu::EdgeTpuContext> edgetpu_contxt;
    std::unique_ptr<tflite::Interpreter> interpreter;
    absl::Span<uint8_t> input;
    std::unordered_map<int, std::string> labels;
    size_t num_devices;

    edgeTPU_USB();
    edgeTPU_USB(const std::string model_path, const std::string labels);

    void set_input();
    void do_inference();
    void get_output();
};


#endif