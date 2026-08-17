#include "motor_control.hpp"
#include "pwm/pwm_drivers.h"
#define MOTOR_CHANNEL 1U

void MotorControl::Init(){
    state = MotorState::STOPPED;
    targetRPM = 0.0f;
    pwmDuty = 0.0f;

    PWM_SetDuty(MOTOR_CHANNEL, 0.0f);
}

void MotorControl::Start(){
    if(state == MotorState::STOPPED){
        state = MotorState::STARTING;
    }
}

void MotorControl::Stop(){
    if(state == MotorState::RUNNING || state == MotorState::STARTING){
        state = MotorState::STOPPING;
    }

    PWM_SetDuty(MOTOR_CHANNEL, 0.0f);
    pwmDuty = 0.0f;
}

void MotorControl::SetDuty(float duty){
    if(duty > 100.0f){
        duty = 100.0f;
    }
    if(duty < 0.0f){
        duty = 0.0f;
    }

    pwmDuty = duty;

    PWM_SetDuty(MOTOR_CHANNEL, pwmDuty);

    if(state == MotorState::STARTING){
        state = MotorState::RUNNING;
    }
}

void MotorControl::SetTargetRPM(float rpm){
    targetRPM = rpm;
}

MotorState MotorControl::GetState() const {
    return state;
}