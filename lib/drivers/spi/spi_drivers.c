#include "spi_drivers.h"
#include "stm32f1xx_hal.h"

SPI_HandleTypeDef hspi;

void SPI_Init(){
    hspi.Instance = SPI1;
    hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi.Init.Mode = SPI_MODE_MASTER;
    hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi.Init.Direction = SPI_DIRECTION_2LINES;
    hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi.Init.NSS = SPI_NSS_SOFT;
    hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi.Init.CRCPolynomial = 7;

    HAL_SPI_Init(&hspi);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi){
    if(hspi->Instance == SPI1){
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitTypeDef gpio = {0};
        gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio);

        gpio.Pin = GPIO_PIN_6;
        gpio.Pull = GPIO_NOPULL;
        gpio.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(GPIOA, &gpio);

        gpio.Pin = GPIO_PIN_4;
        gpio.Mode = GPIO_MODE_OUTPUT_PP;
        gpio.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    }
}