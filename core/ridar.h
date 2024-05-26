#ifndef RIDAR_H
#define RIDAR_H
#include "sl_lidar.h"
#include "sl_lidar_driver.h"
#include "error.h"

#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))

typedef sl_lidar_response_measurement_node_hq_t ridar_data;

extern test ridar_test_level;

class ridar
{
public:
    sl::ILidarDriver* drv;
    sl::Result<sl::IChannel*> channel = nullptr;
    const std::string device = "/dev/ttyUSB0";
    const int baudrate = 115200;
    ridar_data nodes[8192];
    size_t node_count = _countof(nodes);

public:
    ridar();
    ~ridar();

    bool isOpened();

    ridar_data* get_data();

    void scan_start();
    void scan_data();
    float get_distance_from_anlge(float anlge);
};

#endif