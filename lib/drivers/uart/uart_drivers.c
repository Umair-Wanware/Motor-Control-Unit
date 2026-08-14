#include "stm32f1xx_hal.h"
#include "uart_drivers.h"

UART_HandleTypeDef huart;

void UART_Init(){
    huart.Instance = USART1;
    huart.Init.BaudRate = 9600;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart){
    if(huart->Instance == USART1){
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitTypeDef gpio = {0};
        gpio.Pin = GPIO_PIN_9;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio);

        gpio.Pin = GPIO_PIN_10;
        gpio.Mode = GPIO_MODE_INPUT;
        gpio.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &gpio);
    }
}