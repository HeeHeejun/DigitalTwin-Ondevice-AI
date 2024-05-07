#ifndef PRODUCER_H
#define PRODUCER_H

#include "error.h"
#include "camera.h"
#include "ridar.h"
#include "object_detection.h"

test producer_test_level = TEST;

class producer
{
    std::unique_ptr<camera> camera_sensor;
    std::unique_ptr<ridar> ridar_sensor;
    
    producer();
    ~producer();

    void produce();
};

#endif