/*
 * Place callback functions for interrupts in this file 
 */

#include "main.h"
#include "shared_variables.h"

#ifdef __cplusplus
extern "C" {
#endif

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  static uint32_t last_press_ms = 0;
  if (GPIO_Pin == GPIO_PIN_13) {
    uint32_t now = HAL_GetTick();

    if ((now - last_press_ms) > 200) {
      last_press_ms = now;
      printf("User button pressed\n");
    //   CycleLEDs();
    }
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle) {
	if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &headerFIFO0, dataFIFO0) != HAL_OK) {
		printf("Failed to retrieve CAN message!");
		Error_Handler();
	} else {
		// Do something because of message received
		msg_received = 1;
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *CanHandle) {
	if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO1, &headerFIFO1, dataFIFO1) != HAL_OK) {
		printf("Failed to retrieve CAN message!");
		Error_Handler();
	} else {
		// Do something because of message received
		msg_received = 1;
	}
}

#ifdef __cplusplus
}
#endif