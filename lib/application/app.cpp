#include "app.hpp"
#include "adc/adc_drivers.h"
#include "encoder/encoder_drivers.h"
#include "i2c/i2c_drivers.h"
#include "pwm/pwm_drivers.h"
#include "spi/spi_drivers.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_fonts.h"
#include "uart/uart_drivers.h"
#include "sensor_packet.hpp"
#include "sensors.hpp"
#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

volatile uint32_t idleCounter = 0;

extern "C" {
    void vApplicationIdleHook(){
        idleCounter++;
        __WFI();
    }

    void vApplicationMallocFailedHook(){
        taskDISABLE_INTERRUPTS();
        while(1);
    }
}

class SensorTask {
    public:
    SensorTask(QueueHandle_t sQ, QueueHandle_t dQ) : sensorQueue(sQ), displayQueue(dQ) {}

    void Start(){
        xTaskCreate(task_entry, "SENSORS", 512, this, 5, nullptr);
    }

    private:
    QueueHandle_t sensorQueue, displayQueue;
    static void task_entry(void *pvPara){
        static_cast<SensorTask*>(pvPara)->Run();
    }

    void Run(){
        SensorPacket packet;

        TickType_t lastWakeTime = xTaskGetTickCount();
        TickType_t lastCurrent = lastWakeTime;
        TickType_t lastVoltage = lastWakeTime;
        TickType_t lastTemperature = lastWakeTime;
        TickType_t lastEncoder = lastWakeTime;

        while(true){
            TickType_t now = xTaskGetTickCount();

            if((now - lastCurrent) >= pdMS_TO_TICKS(1)){
                lastCurrent = now;

                uint16_t rawCurrent = ADC_Read(ADC_CHANNEL_0);
                packet.current = Sensors::curr_Read(rawCurrent);
            }

            if((now - lastEncoder) >= pdMS_TO_TICKS(5)){
                lastEncoder = now;

                Encoder_UpdateRPM();
                packet.encoder_count = Encoder_GetPosition();
                packet.rpm = Encoder_GetRPM();
            }

            if((now - lastVoltage) >= pdMS_TO_TICKS(10)){
                lastVoltage = now;

                uint16_t rawVoltage = ADC_Read(ADC_CHANNEL_1);
                packet.voltage = Sensors::vol_Read(rawVoltage);
            }

            if((now - lastTemperature) >= pdMS_TO_TICKS(100)){
                lastTemperature = now;

                uint16_t rawTemperature = ADC_Read(ADC_CHANNEL_2);
                packet.temperature = Sensors::temp_Read(rawTemperature);
            }

            packet.fault = 0;

            if(packet.voltage < 10.0f) packet.fault |= 0x01;
            if(packet.current > 20.0f) packet.fault |= 0x02;
            if(packet.temperature > 80.0f) packet.fault |= 0x04;

            xQueueOverwrite(sensorQueue, &packet);
            xQueueOverwrite(displayQueue, &packet);

            vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));
        }
    }
};

void App_Init(){
    UART_Init();
    SPI_Init();
    I2C_Init();
    ADC_Init();
    PWM_Init();
    Encoder_Init();
    ssd1306_Init();

    QueueHandle_t sensorQueue = xQueueCreate(1, sizeof(SensorPacket));
    QueueHandle_t displayQueue = xQueueCreate(1, sizeof(SensorPacket));

    configASSERT(sensorQueue);
    configASSERT(displayQueue);

    static SensorTask sensortask(sensorQueue, displayQueue);

    sensortask.Start();

    vTaskStartScheduler();

    while(true);
}