#include "stm32f1xx_hal.h"
#include "SystemClockConfig.h"
#include "app.hpp"

int main(void){
    HAL_Init();
    SystemClock_Config();
    App_Init();

    while(true);
}