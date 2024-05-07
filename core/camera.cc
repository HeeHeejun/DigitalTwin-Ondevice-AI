#include "camera.h"

test camera_test_level = TEST;

camera::camera()
{
    cap[FRONT] = cv::VideoCapture(0, cv::CAP_V4L2); 
    cap[LEFT] = cv::VideoCapture(2, cv::CAP_V4L2);
    cap[LIGHT] = cv::VideoCapture(4, cv::CAP_V4L2);
    cap[BACK] = cv::VideoCapture(6, cv::CAP_V4L2);
    if(!(cap[0].isOpened()&&cap[1].isOpened()&&cap[2].isOpened()&&cap[3].isOpened())) print(CAMERA_ERROR);
    for(int i = 0; i < 4; i++)
    {
        cap[i].set(cv::CAP_PROP_FRAME_WIDTH, 1920);
        cap[i].set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    }
    pictures.resize(4);
}

camera::~camera()
{
    for(int i = 0; i < 4; i++)
        cap[i].release();
}

error_type camera::do_capture()
{
    std::thread camera_thread[4];
    for(int i = 0; i < 4; i++)
        camera_thread[i] = std::thread(capture_per_camera, std::ref(cap[i]), std::ref(pictures), static_cast<camera_location>(i));
    camera_thread[FRONT].join();
    camera_thread[LEFT].join();
    camera_thread[LIGHT].join();
    camera_thread[BACK].join();
    cap_queue.push(pictures);
    
    if(camera_test_level)
        std::cout << "picture_size" <<cap_queue.size()<< "\n";

    return DONE;
}

error_type camera::capture_per_camera(cv::VideoCapture video, std::vector<cv::Mat>& pictures, camera_location type)
{
    cv::Mat temp;
    video >> temp;
    if(temp.empty()) return CAMERA_ERROR;

    pictures[type] = temp;

    if (camera_test_level)
    {
        fs::create_directories("output");
        camera::test_view(std::ref(pictures), type);
    }

    return DONE;
}

error_type camera::test_view(std::vector<cv::Mat>& pictures, camera_location type)
{
    std::string filename = "output/frame_" + std::to_string(type) + ".bmp";
    cv::imwrite(filename, pictures[type]);
    std::cout << "Saved " << filename << std::endl;

    return DONE;
}
