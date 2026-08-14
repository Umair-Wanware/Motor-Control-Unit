#ifndef SPI_DRIVERS_H
#define SPI_DRIVERS_H

#include "stm32f1xx_hal.h"
extern SPI_HandleTypeDef hspi;

#ifdef __cplusplus
extern "C" {
    #endif
    void SPI_Init();
    void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi);
    #ifdef __cplusplus
}
#endif
#endif