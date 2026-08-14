#include "encoder_drivers.h"

TIM_HandleTypeDef htim2;

static int32_t previousCount = 0;
static int32_t rpm = 0;

#define ENCODER_PPR 1024
#define SAMPLE_TIME_MS 100

void Encoder_Init(void){
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;

    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    TIM_Encoder_InitTypeDef encoder = {0};

    encoder.EncoderMode = TIM_ENCODERMODE_TI12;

    encoder.IC1Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    encoder.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    encoder.IC1Prescaler = TIM_ICPSC_DIV1;
    encoder.IC1Filter = 5;

    encoder.IC2Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    encoder.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    encoder.IC2Prescaler = TIM_ICPSC_DIV1;
    encoder.IC2Filter = 5;

    HAL_TIM_Encoder_Init(&htim2, &encoder);
}

void Encoder_Start(void){
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
}

void Encoder_Stop(void){
    HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
}

int32_t Encoder_GetPosition(void){
    return (int32_t)__HAL_TIM_GET_COUNTER(&htim2);
}

void Encoder_Reset(void){
    __HAL_TIM_SetCounter(&htim2, 0);
    previousCount = 0;
}

int32_t Encoder_GetDelta(void){
    int32_t current = Encoder_GetPosition();
    int32_t delta = current - previousCount;
    previousCount = current;

    return delta;
}

void Encoder_UpdateRPM(void){
    int32_t delta = Encoder_GetDelta();
    rpm = (delta * 600) / (ENCODER_PPR * SAMPLE_TIME_MS);
}

int32_t Encoder_GetRPM(void){
    return rpm;
}