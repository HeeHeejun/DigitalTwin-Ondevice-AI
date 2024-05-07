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
        //camera의 do_capture();
        //결과를 pop해서 맨앞 큐에있는 것을 들고옴
        //ridar 센서의 정보 받아옴, camera와 그 결과를 동시에 끝냄
        //detect model 의 input setting
        //tpu 추론과정
        //tpu의 output 받아옴
        //ridar 센서의 값과 camera에서의 추론 결과를 통해 -> angle을 통해 맞춰주는 과정이 필요
        //그 값을 보내줌
    }
}