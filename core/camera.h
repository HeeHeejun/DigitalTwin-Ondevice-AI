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
    std::queue<std::vector<uint8_t>> cap_queue;
    const int num_camera = 4;

public:
    camera();
    ~camera();
public:
    error_type do_capture();

    std::vector<uint8_t>& pop_cap();

    error_type camera::capture_per_camera(cv::VideoCapture& video, std::vector<uint8_t>& pictures, const camera_location& type);

    error_type camera::test_view(const cv::Mat& pictures, const camera_location type);
};

#endif