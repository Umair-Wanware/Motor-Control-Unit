#include "sensors.hpp"

namespace Sensors
{

float curr_Read(uint16_t adc)
{
    return ((float)adc * 3.3f / 4095.0f - 1.65f) / 0.066f;
}

float vol_Read(uint16_t adc)
{
    return ((float)adc * 3.3f / 4095.0f) * 11.0f;
}

float temp_Read(uint16_t adc)
{
    float voltage = (float)adc * 3.3f / 4095.0f;
    return voltage * 100.0f;
}

}