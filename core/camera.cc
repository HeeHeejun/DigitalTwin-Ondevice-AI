#include "camera.h"

test camera_test_level = TEST;

camera::camera()
{
    cap[FRONT] = cv::VideoCapture(0, cv::CAP_V4L2); 
    cap[LEFT] = cv::VideoCapture(2, cv::CAP_V4L2);
    cap[LIGHT] = cv::VideoCapture(4, cv::CAP_V4L2);
    cap[BACK] = cv::VideoCapture(6, cv::CAP_V4L2);
    if(!(cap[0].isOpened()&&cap[1].isOpened()&&cap[2].isOpened()&&cap[3].isOpened())) 
        std::cerr << CAMERA_ERROR << std::endl;
    for(int i = 0; i < 4; i++)
    {
        cap[i].set(cv::CAP_PROP_FRAME_WIDTH, 1920);
        cap[i].set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    }
}

camera::~camera()
{
    for(int i = 0; i < num_camera; i++)
        cap[i].release();
}

error_type camera::do_capture()
{
    std::vector<std::thread> camera_thread;
    std::vector<std::vector<uint8_t>> pictures = std::vector<std::vector<uint8_t>>(num_camera);
    for(int i = 0; i < num_camera; i++)
        //camera_thread.emplace_back(&camera::capture_per_camera,std::ref(cap[i]), std::ref(pictures), static_cast<camera_location>(i));
        camera_thread.emplace_back(std::bind(&camera::capture_per_camera, this, std::ref(cap[i]), std::ref(pictures[i]), static_cast<camera_location>(i)));
    
    for(auto& thread : camera_thread)
        thread.join();

    cap_queue.push(pictures);
    
    if(camera_test_level)
        std::cout << "picture_size" << cap_queue.size() << "\n";

    return DONE;
}

std::vector<std::vector<uint8_t>> camera::pop_cap()
{
    auto tmp = cap_queue.front();
    cap_queue.pop();
    return tmp;
}

error_type camera::capture_per_camera(cv::VideoCapture& video, std::vector<uint8_t>& pictures, const camera_location type)
{
    cv::Mat temp;
    video >> temp;
    if(temp.empty()) return CAMERA_ERROR;

    if (temp.isContinuous()) 
        pictures.assign(temp.data, temp.data + temp.total() * temp.channels());
    else 
    {
        for (int i = 0; i < temp.rows; ++i) 
            pictures.insert(pictures.end(), temp.ptr<uint8_t>(i), temp.ptr<uint8_t>(i) + temp.cols * temp.channels());
    }

    if (camera_test_level)
        test_view(std::ref(temp), type);
    return DONE;
}

error_type camera::test_view(const cv::Mat& pictures, const camera_location type)
{
    std::string filename = "output/frame_" + std::to_string(type) + ".bmp";
    cv::imwrite(filename, pictures);
    std::cout << "Saved " << filename << std::endl;

    return DONE;
}
