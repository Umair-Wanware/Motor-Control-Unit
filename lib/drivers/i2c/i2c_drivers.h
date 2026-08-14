#ifndef I2C_DRIVERS_H
#define I2C_DRIVERS_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
    #endif
    extern I2C_HandleTypeDef hi2c;
    void I2C_Init();
    void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
    #ifdef __cplusplus
}
#endif
#endif