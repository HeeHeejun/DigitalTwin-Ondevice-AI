#include "ridar.h"

test ridar_test_level = TEST;

ridar::ridar()
{
    drv = *sl::createLidarDriver();
    channel = sl::createSerialPortChannel(device, baudrate);

    auto res = drv->connect(*channel);
    if(SL_IS_OK(res) && ridar_test_level)
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
    delete drv;
    delete *channel;
}

error_type ridar::scan_start()
{ 
    auto ans = drv->startScan(1, 1); 
    if(!ans) return RIDAR_ERROR;

    return DONE;
}

error_type ridar::scan_data()
{
    auto ans = drv->grabScanDataHq(nodes, node_count);
    if (!SL_IS_OK(ans)) return RIDAR_ERROR;
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

    return DONE;
}

ridar_data* ridar::get_data(){return nodes;}