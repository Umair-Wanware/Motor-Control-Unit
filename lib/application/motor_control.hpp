#pragma once
#include "stm32f1xx_hal.h"
#include <stdint.h>

enum class MotorState {
    STOPPED = 0,
    STARTING,
    RUNNING,
    STOPPING,
    FAULT
};

class MotorControl {
    public:
    void Init();
    void Start();
    void Stop();

    void SetDuty(float duty);
    void SetTargetRPM(float rpm);

    MotorState GetState() const;

    private:
    MotorState state = MotorState::STOPPED;
    float targetRPM = 0.0f;
    float pwmDuty = 0.0f;
};