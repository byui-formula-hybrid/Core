#ifndef __SHARED_VARIABLES_H
#define __SHARED_VARIABLES_H

#include "main.h"

extern CAN_RxHeaderTypeDef headerFIFO0;
extern CAN_RxHeaderTypeDef headerFIFO1;
extern uint8_t dataFIFO0[8];
extern uint8_t dataFIFO1[8];
extern volatile uint8_t msg_received;
extern volatile uint8_t button_pressed;
extern volatile uint32_t last_press_ms;

#endif /* __SHARED_VARIABLES_H */