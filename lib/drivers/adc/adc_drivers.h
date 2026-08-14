#ifndef ADC_DRIVERS_H
#define ADC_DRIVERS_H

#include "stm32f1xx_hal.h"
extern ADC_HandleTypeDef hadc;
#ifdef __cplusplus
extern "C" {
    #endif
    void ADC_Init();
    uint16_t ADC_Read(uint32_t channel);
    void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
    #ifdef __cplusplus
}
#endif
#endif