#include "producer.h"

producer::producer()
{
    camera_sensor = std::make_unique<camera>();
    ridar_sensor = std::make_unique<ridar>();
}

void producer::produce()
{
    int count;
    if(producer_test_level) count = 100;
    while(count--)
    {
        camera_sensor.do_capture();
        //tpu 추론
        
    }
}