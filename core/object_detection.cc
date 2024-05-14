#include "object_detection.h"

edgeTPU_USB::edgeTPU_USB(const std::string& model_path, const std::string& labels)
{
    model = std::move(coral::LoadModelOrDie(model_path));
    edgetpu_context = coral::ContainsEdgeTpuCustomOp(*model) ? coral::GetEdgeTpuContextOrDie() : nullptr;
    interpreter = coral::MakeEdgeTpuInterpreterOrDie(*model, edgetpu_context.get());
    label = coral::ReadLabelFile(labels);
    int num_pp_count = 0;
    for (int i = 0; i < interpreter->nodes_size(); ++i) 
    {
        const char* custom_name = interpreter->node_and_registration(i)->second.custom_name;
        if (custom_name && std::strcmp(custom_name, "TFLite_Detection_PostProcess") == 0) 
        {
            ++num_pp_count;
        }
    }

    CHECK_EQ(interpreter->AllocateTensors(), kTfLiteOk);
    CHECK_EQ(interpreter->inputs().size(), 1);
    const auto* input_tensor = interpreter->input_tensor(0);
    CHECK_EQ(input_tensor->type, kTfLiteUInt8);
}

void edgeTPU_USB::do_inference(std::vector<uint8_t>& input_img)
{
    if(edgeTPU_test_level)
    {
        CHECK_EQ(coral::)
    }
    CHECK_EQ(coral::InvokeWithMemBuffer(interpreter.get(), input_img.data(), input_img.size()), absl::OkStatus());
}

std::vector<Object>& edgeTPU_USB::get_output()
{
    std::vector<Object> results;
    for (auto result : coral::GetDetectionResults(*interpreter, score_threshold)) 
    {
        Object tmp(result.id, labels[result.id], Box(result.bbox.ymin, result.bbox.xmin, result.bbox.ymax, result.bbox.xmax));
        results.push_back(tmp);
    }
    return results;
}