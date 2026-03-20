#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include "main.h"

#ifdef ENV_STM32
// For STM32 specific variables if any
// same model for esp32 specific variables
#endif
extern volatile uint8_t msg_received;
extern volatile uint8_t button_pressed;
extern volatile uint32_t last_press_ms;

// TaskHandle_t xCANProducerHandle;
// TaskHandle_t xCANConsumerHandle;
extern QueueHandle_t xCANRxQueue;
extern QueueHandle_t xCANTxQueue;

typedef struct {
    uint32_t StdID;
    uint32_t ExtID;
    uint32_t RTR;
    uint32_t DLC;
    uint32_t IDE;
    uint8_t Data[8];
} CAN_Msg_t;

#endif /* SHARED_VARIABLES_H */