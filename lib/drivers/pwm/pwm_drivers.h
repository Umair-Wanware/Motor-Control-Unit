#ifndef PWM_DRIVERS_H
#define PWM_DRIVERS_H

#ifdef __cplusplus
extern "C" {
    #endif

    #include "stm32f1xx_hal.h"
    #include <stdint.h>

    void PWM_Init(void);

    void PWM_Start(uint32_t channel);
    void PWM_Stop(uint32_t channel);

    void PWM_SetDuty(uint32_t, float duty);
    float PWM_GetDuty(uint32_t channel);

    void PWM_SetFrequency(uint32_t frequency);

    extern TIM_HandleTypeDef htim3;

    #ifdef __cplusplus
}
#endif
#endif