#include "safety.hpp"
#include "stm32f1xx_hal.h"

bool Safety::checkCurrent(float current){
    if(current > 20.0f){
        fault = true;
        return false;
    }
    return true;
}

bool Safety::checkVoltage(float voltage){
    if(voltage < 10.0f || voltage > 30.0f){
        fault = true;
        return false;
    }
    return true;
}

bool Safety::checkTemperature(float temperature){
    if(temperature > 80.0f){
        fault = true;
        return false;
    }
    return true;
}

bool Safety::Fault() const {
    return fault;
}