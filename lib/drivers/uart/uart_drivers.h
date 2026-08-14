#ifndef UART_DRIVERS_H
#define UART_DRIVERS_H

#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart;
#ifdef __cplusplus
extern "C" {
#endif
    void UART_Init();
    void HAL_UART_MspInit(UART_HandleTypeDef* huart);
#ifdef __cplusplus
}
#endif
#endif