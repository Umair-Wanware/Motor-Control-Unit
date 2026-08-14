#include "pwm_drivers.h"

TIM_HandleTypeDef htim3;

static float dutyCycle[4] = {};

void PWM_Init(void){
    __HAL_RCC_TIM3_CLK_ENABLE();

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 3599;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_PWM_Init(&htim3);

    TIM_OC_InitTypeDef config = {};

    config.OCMode = TIM_OCMODE_PWM1;
    config.Pulse = 0;
    config.OCPolarity = TIM_OCPOLARITY_HIGH;
    config.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim3, &config, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim3, &config, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim3, &config, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &config, TIM_CHANNEL_4);
}

void PWM_Start(uint32_t channel){
    HAL_TIM_PWM_Start(&htim3, channel);
}

void PWM_Stop(uint32_t channel){
    HAL_TIM_PWM_Stop(&htim3, channel);
}

void PWM_SetDuty(uint32_t channel, float duty){
    if(duty < 0.0f) duty = 0.0f;
    if(duty > 100.0f) duty = 100.0f;

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3);
    uint32_t compare = (uint32_t)((duty * (arr + 1)) / 100.0f);

    __HAL_TIM_SET_COMPARE(&htim3, channel, compare);

    switch(channel){
        case TIM_CHANNEL_1 :
            dutyCycle[0] = duty;
            break;
        case TIM_CHANNEL_2 :
            dutyCycle[1] = duty;
            break;
        case TIM_CHANNEL_3:
            dutyCycle[2] = duty;
            break;
        case TIM_CHANNEL_4:
            dutyCycle[3] = duty;
            break;
    }
}

float PWM_GetDuty(uint32_t channel){
        switch(channel){
            case TIM_CHANNEL_1: return dutyCycle[0];
            case TIM_CHANNEL_2: return dutyCycle[1];
            case TIM_CHANNEL_3: return dutyCycle[2];
            case TIM_CHANNEL_4: return dutyCycle[3];
            default: return 0.0f;
        }
}

void PWM_SetFrequency(uint32_t frequency){
    if(frequency == 0) return;

    uint32_t timerClock = 72000000UL;
    uint32_t arr = (timerClock / frequency) - 1;

    __HAL_TIM_SetAutoreload(&htim3, arr);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}