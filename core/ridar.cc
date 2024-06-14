#include "ridar.h"
#include <cmath>
test ridar_test_level = NONE;

ridar::ridar()
{
    drv = *sl::createLidarDriver();
    channel = sl::createSerialPortChannel(device, baudrate);

    auto res = drv->connect(*channel);
    if(SL_IS_OK(res))
    {
        sl_lidar_response_device_info_t deviceInfo;
        res = drv->getDeviceInfo(deviceInfo);
        if(SL_IS_OK(res))
            printf("Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n", deviceInfo.model, deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xffu, deviceInfo.hardware_version);        
        else
            fprintf(stderr, "Failed to get device information from LIDAR %08x\r\n", res);
    }
    else
        fprintf(stderr, "Failed to connect to LIDAR %08x\r\n", res);

    scan_start();
}

ridar::~ridar()
{
    drv->stop();
    drv->setMotorSpeed(0);
    delete drv;
    delete *channel;
}

void ridar::scan_start()
{ 
    auto ans = drv->startScan(1, 1); 
    if(!ans) return;

    return;
}

void ridar::scan_data()
{
    auto ans = drv->grabScanDataHq(nodes, node_count);
    if (!SL_IS_OK(ans)) return;
    drv->ascendScanData(nodes, node_count);

    if(ridar_test_level)
    {
        for (int pos = 0; pos < (int)node_count; ++pos)
        {
            printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
            (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
            (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
            nodes[pos].dist_mm_q2/4.0f,
            nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
        }
    }

    return;
}

float ridar::get_distance_from_anlge(float anlge)
{
    for (int pos = 0; pos < (int)node_count; ++pos)
    {
        if(nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT != 0)
        {   
            //std::cout << "angle : " << (int)anlge << " lidar : " << (int)((nodes[pos].angle_z_q14 * 90.f) / 16384.f) << "\n";
            if(abs(anlge - (((nodes[pos].angle_z_q14 * 90.f) / 16384.f))) < 1.5 )
            {
                if(nodes[pos].dist_mm_q2 / 4.0f < 1000.0f)
                    return nodes[pos].dist_mm_q2 / 4.0f;
            }   
        }
    }
    return 100.0;
}


ridar_data* ridar::get_data(){return nodes;}