#include "i2c_drivers.h"

I2C_HandleTypeDef hi2c;

void I2C_Init(){
    hi2c.Instance = I2C1;
    hi2c.Init.ClockSpeed = 100000;
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_16_9;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c){
    if(hi2c->Instance == I2C1){
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitTypeDef gpio = {0};
        gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &gpio);
    }
}