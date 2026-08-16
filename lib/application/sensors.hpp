#pragma once
#include <stdint.h>

namespace Sensors {
    float curr_Read(uint16_t adc);
    float vol_Read(uint16_t adc);
    float temp_Read(uint16_t adc);
};