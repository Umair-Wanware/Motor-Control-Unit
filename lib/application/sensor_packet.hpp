#pragma once
#include <stdint.h>

typedef struct SensorPacket {
    float temperature, voltage, current;
    uint32_t encoder_count, rpm; 
    uint16_t fault;
} SensorPacket; 