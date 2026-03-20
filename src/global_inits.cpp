#include "shared_variables.h"

// CAN_RxHeaderTypeDef headerFIFO0;
// CAN_RxHeaderTypeDef headerFIFO1;
// uint8_t dataFIFO0[8];
// uint8_t dataFIFO1[8];

volatile uint8_t button_pressed = 0;
volatile uint32_t last_press_ms = 0;
volatile uint8_t msg_received = 0;

QueueHandle_t xCANRxQueue = NULL;
QueueHandle_t xCANTxQueue = NULL;