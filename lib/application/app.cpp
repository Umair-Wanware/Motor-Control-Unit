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
#include "motor_control.hpp"
#include "safety.hpp"
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
    SensorTask(QueueHandle_t sQ, QueueHandle_t dQ, QueueHandle_t sfQ) : sensorQueue(sQ), displayQueue(dQ), safetyQueue(sfQ) {}

    void Start(){
        xTaskCreate(task_entry, "SENSORS", 512, this, 5, nullptr);
    }

    private:
    QueueHandle_t sensorQueue, displayQueue, safetyQueue;
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
            xQueueOverwrite(safetyQueue, &packet);

            vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));
        }
    }
};

class CommunicationTask {
    public:
    CommunicationTask(QueueHandle_t sQ) : sensorQueue(sQ) {}

    void Start(){
        xTaskCreate(task_entry, "COMM", 512, this, 4, nullptr);
    }

    private:
    QueueHandle_t sensorQueue;
    static void task_entry(void *pvPara){
        static_cast<CommunicationTask*>(pvPara)->Run();
    }

    void Run(){
        SensorPacket packet;

        while(true){
            if(xQueueReceive(sensorQueue, &packet, portMAX_DELAY) == pdPASS){
                HAL_SPI_Transmit(&hspi, reinterpret_cast<uint8_t*>(&packet), sizeof(packet), 1000);
            }
        }
    }
};

class DisplayTask {
    public:
    DisplayTask(QueueHandle_t dQ) : displayQueue(dQ) {}

    void Start(){
        xTaskCreate(task_entry, "DISP", 512, this, 3, nullptr);
    }

    private:
    QueueHandle_t displayQueue;
    static void task_entry(void *pvPara){
        static_cast<DisplayTask*>(pvPara)->Run();
    }

    void Run(){
        SensorPacket packet;
        char buffer[48];

        TickType_t laskWakeTime = xTaskGetTickCount();
        const TickType_t period = pdMS_TO_TICKS(200);

        while(true){
            xQueuePeek(displayQueue, &packet, 0);

            ssd1306_Fill(Black);

            snprintf(buffer, sizeof(buffer), "RPM : %4lu", (unsigned long)packet.rpm);
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString(buffer, Font_7x10, White);

            snprintf(buffer, sizeof(buffer), "V   : %.2f", packet.voltage);
            ssd1306_SetCursor(0, 16);
            ssd1306_WriteString(buffer, Font_7x10, White);

            snprintf(buffer, sizeof(buffer), "I   : %.2f", packet.current);              
            ssd1306_SetCursor(0, 32);
            ssd1306_WriteString(buffer, Font_7x10, White);

            snprintf(buffer, sizeof(buffer), "T   : %.1f C", packet.temperature);
            ssd1306_SetCursor(0, 48);
            ssd1306_WriteString(buffer, Font_7x10, White);

            ssd1306_UpdateScreen();

            vTaskDelayUntil(&laskWakeTime, period);
        }
    }
};

class MotorTask {
    public:
    MotorTask(QueueHandle_t sQ) : sensorQueue(sQ) {}

    void Start(){
        xTaskCreate(task_entry, "MOTOR", 512, this, 2, nullptr);
    }

    private:
    QueueHandle_t sensorQueue;
    MotorControl motor;

    static void task_entry(void *pvPara){
        static_cast<MotorTask*>(pvPara)->Run();
    }

    void Run(){
        SensorPacket packet;

        motor.Init();
        motor.SetTargetRPM(1500.0f);
        motor.Start();

        float duty = 0.0f;

        TickType_t lastWakeTime = xTaskGetTickCount();
        const TickType_t period = pdMS_TO_TICKS(1);

        while(true){
            if(xQueuePeek(sensorQueue, &packet, 0) == pdPASS){
                switch(motor.GetState()){
                    case MotorState::STOPPED:
                        duty = 0.0f;
                        motor.SetDuty(duty);
                        break;
                    case MotorState::STARTING:
                        duty += 0.5f;
                        if(duty > 20.0f) duty = 20.0f;
                        motor.SetDuty(duty);
                        break;
                    case MotorState::RUNNING:
                        motor.SetDuty(duty);
                        break;
                    case MotorState::STOPPING:
                        duty -= 0.5f;
                        if(duty < 0.0f) duty = 0.0f;
                        motor.SetDuty(duty);
                        break;
                    case MotorState::FAULT:
                        duty = 0.0f;
                        motor.SetDuty(0.0f);
                        break;
                }
                if(packet.temperature > 80.0f) motor.Stop();
                if(packet.current > 20.0f) motor.Stop();
                if(packet.voltage < 10.0f) motor.Stop();
            }
            vTaskDelayUntil(&lastWakeTime, period);
        }
    }
};

class SafetyTask {
    public:
    SafetyTask(QueueHandle_t sQ) : safetyQueue(sQ) {}

    void Start(){
        xTaskCreate(task_entry, "SAFETY", 512, this, 1, nullptr);
    }

    private:
    QueueHandle_t safetyQueue;
    MotorControl motor;

    static void task_entry(void *pvPara){
        static_cast<SafetyTask*>(pvPara)->Run();
    }

    void Run(){
        SensorPacket packet;
        Safety safety;

        TickType_t lastWakeTime = xTaskGetTickCount();
        const TickType_t period = pdMS_TO_TICKS(5);

        while(true){
            if(xQueuePeek(safetyQueue, &packet, 0) == pdPASS){
                safety.checkCurrent(packet.current);
                safety.checkVoltage(packet.voltage);
                safety.checkTemperature(packet.temperature);

                if(safety.Fault()){
                    motor.Stop();
                }
            }
        }
        vTaskDelayUntil(&lastWakeTime, period);
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
    QueueHandle_t safetyQueue = xQueueCreate(1, sizeof(SensorPacket));

    configASSERT(sensorQueue);
    configASSERT(displayQueue);
    configASSERT(safetyQueue);

    static SensorTask sensortask(sensorQueue, displayQueue, safetyQueue);
    static CommunicationTask communicationtask(sensorQueue);
    static DisplayTask displaytask(displayQueue);
    static MotorTask motortask(sensorQueue);
    static SafetyTask safetytask(safetyQueue);

    sensortask.Start();
    communicationtask.Start();
    displaytask.Start();
    motortask.Start();
    safetytask.Start();

    vTaskStartScheduler();

    while(true);
}