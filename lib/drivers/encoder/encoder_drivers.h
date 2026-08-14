#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

#ifdef __cplusplus
extern "C" {
    #endif

    #include "stm32f1xx_hal.h"
    #include <stdint.h>

    void Encoder_Init(void);
    void Encoder_Start(void);
    void Encoder_Stop(void);

    int32_t Encoder_GetPosition(void);
    int32_t Encoder_GetDelta(void);

    void Encoder_Reset(void);

    int32_t Encoder_GetRPM(void);
    void Encoder_UpdateRPM(void);

    #ifdef __cplusplus
}
#endif
#endif