#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>
#include <thread>
#include <filesystem>
#include "error.h"

namespace fs = std::filesystem;

enum camera_location
{
    FRONT,
    LEFT,
    LIGHT,
    BACK,
};

extern test camera_test_level;

//test test_level = TEST;

class camera
{
public:
    cv::VideoCapture cap[4];
    std::queue<std::vector<cv::Mat>> cap_queue;
    std::vector<cv::Mat> pictures;

public:
    camera();
    ~camera();
public:
    error_type do_capture();

    static error_type capture_per_camera(cv::VideoCapture video, std::vector<cv::Mat>& pictures, camera_location type);

    static error_type test_view(std::vector<cv::Mat>& pictures, camera_location type);
};

#endif