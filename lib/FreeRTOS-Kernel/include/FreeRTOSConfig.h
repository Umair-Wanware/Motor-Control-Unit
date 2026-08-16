#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* ============================================================
 * STM32F103RBT6
 * Cortex-M3
 * 72 MHz CPU
 * 20 KB SRAM
 * ============================================================ */


/* ---------------- Kernel behaviour ---------------- */

#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                   1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0

#define configCPU_CLOCK_HZ                       ((uint32_t)72000000)
#define configTICK_RATE_HZ                       ((TickType_t)1000)

#define configMAX_PRIORITIES                     8
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
#define configMAX_TASK_NAME_LEN                  16

#define configUSE_16_BIT_TICKS                   0
#define configIDLE_SHOULD_YIELD                  1


/* ---------------- Memory ---------------- */

/*
 * STM32F103RB has only 20 KB SRAM.
 *
 * Your application also uses:
 * - HAL
 * - queues
 * - SSD1306
 * - application globals
 * - task stacks
 *
 * Start with 10 KB for FreeRTOS heap.
 */
#define configTOTAL_HEAP_SIZE                    ((size_t)(2 * 1024))

#define configSUPPORT_STATIC_ALLOCATION          0
#define configSUPPORT_DYNAMIC_ALLOCATION         1

#define configAPPLICATION_ALLOCATED_HEAP         0


/* ---------------- Hooks ---------------- */

#define configUSE_IDLE_HOOK                      1
#define configUSE_TICK_HOOK                      0

#define configCHECK_FOR_STACK_OVERFLOW           0
#define configUSE_MALLOC_FAILED_HOOK             1


/* ---------------- Synchronisation ---------------- */

#define configUSE_MUTEXES                        1
#define configUSE_RECURSIVE_MUTEXES              1
#define configUSE_COUNTING_SEMAPHORES            1

#define configUSE_QUEUE_SETS                     0


/* ---------------- Task notifications ---------------- */

#define configUSE_TASK_NOTIFICATIONS             1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES    1


/* ---------------- Software timers ---------------- */

#define configUSE_TIMERS                         1

#define configTIMER_TASK_PRIORITY                2
#define configTIMER_QUEUE_LENGTH                 5
#define configTIMER_TASK_STACK_DEPTH             256


/* ---------------- Runtime statistics ---------------- */

#define configGENERATE_RUN_TIME_STATS            0
#define configUSE_TRACE_FACILITY                 0
#define configUSE_STATS_FORMATTING_FUNCTIONS     0


/* ---------------- Co-routines ---------------- */

#define configUSE_CO_ROUTINES                    0
#define configMAX_CO_ROUTINE_PRIORITIES          1


/* ---------------- Newlib ---------------- */

#define configUSE_NEWLIB_REENTRANT               0


/* ---------------- Assertions ---------------- */

#define configASSERT(x)                          \
    do                                           \
    {                                            \
        if ((x) == 0)                            \
        {                                        \
            taskDISABLE_INTERRUPTS();            \
            for (;;)                             \
            {                                    \
            }                                    \
        }                                        \
    } while (0)


/* ============================================================
 * Cortex-M3 interrupt configuration
 * ============================================================ */

#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS 4
#endif

/*
 * STM32F1:
 * lower numerical value = higher interrupt priority.
 *
 * FreeRTOS kernel interrupts use the lowest priority.
 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY          15

/*
 * ISRs using FreeRTOS FromISR APIs must have a numerical
 * priority >= 5.
 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY     5

#define configKERNEL_INTERRUPT_PRIORITY                  \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configMAX_SYSCALL_INTERRUPT_PRIORITY             \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))


/* ============================================================
 * FreeRTOS API inclusion
 * ============================================================ */

#define INCLUDE_vTaskPrioritySet                 1
#define INCLUDE_uxTaskPriorityGet                1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_vTaskDelay                       1
#define INCLUDE_xTaskGetSchedulerState           1
#define INCLUDE_xTaskGetCurrentTaskHandle        1
#define INCLUDE_xTaskGetIdleTaskHandle           1
#define INCLUDE_uxTaskGetStackHighWaterMark      1

#endif /* FREERTOS_CONFIG_H */