#include "stm32f1xx_hal.h"
#include "adc_drivers.h"

ADC_HandleTypeDef hadc;

void ADC_Init(){
    hadc.Instance = ADC1;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.NbrOfConversion = 3;
    HAL_ADC_Init(&hadc);
}

uint16_t ADC_Read(uint32_t channel){
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);

    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
    uint16_t value = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
    return value;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc){
    if(hadc->Instance == ADC1){
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitTypeDef gpio = {0};
        gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3;
        gpio.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &gpio);
    }
}