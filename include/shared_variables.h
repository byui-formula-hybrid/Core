#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include "main.h"

#ifdef ENV_STM32
// extern CAN_RxHeaderTypeDef headerFIFO0;
// extern CAN_RxHeaderTypeDef headerFIFO1;
#endif
// extern uint8_t dataFIFO0[8];
// extern uint8_t dataFIFO1[8];
extern volatile uint8_t msg_received;
extern volatile uint8_t button_pressed;
extern volatile uint32_t last_press_ms;

TaskHandle_t xCANProducerHandle;
TaskHandle_t xCANConsumerHandle;
QueueHandle_t xCANRxQueue;
QueueHandle_t xCANTxQueue;

typedef struct {
    uint32_t StdID;
    uint32_t ExtID;
    uint32_t RTR;
    uint32_t DLC;
    uint32_t IDE;
    uint8_t Data[8];
} CAN_Msg_t;

#endif /* __SHARED_VARIABLES_H */