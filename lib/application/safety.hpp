#pragma once
#include <stdio.h>

class Safety {
    public:
    bool checkCurrent(float current);
    bool checkVoltage(float voltage);
    bool checkTemperature(float temperature);

    bool Fault() const;

    private:
    bool fault = false;
};