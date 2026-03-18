/**
 * Place callback functions for interrupts in this file
 * This is part of the bridge between the hardware and freertos
 * These callbacks should dump the relevant information (CAN messages, UART, etc.) into the shared variables and trigger the relevant task
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
    }
  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle) {
	CAN_RxHeaderTypeDef header;
	// uint8_t data[8];
	CAN_Msg_t msg;
	BaseType_t xHigherPriorityTask = pdFALSE;
	if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &header, msg.Data) != HAL_OK) {
		printf("Failed to retrieve CAN message!");
		Error_Handler();
	} else {
		// Do something because of message received
		msg.DLC = header.DLC;
		msg.StdID = header.StdId;
		msg.RTR = header.RTR;
		msg.IDE = header.IDE;
		msg.ExtID = header.ExtId;
		xQueueSendFromISR(xCANRxQueue, &msg, &xHigherPriorityTask);
		// msg_received = 1;
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *CanHandle) {
	CAN_RxHeaderTypeDef header;
	CAN_Msg_t msg;
	BaseType_t xHigherPriorityTask = pdFALSE;
	if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO1, &header, msg.Data) != HAL_OK) {
		printf("Failed to retrieve CAN message!");
		Error_Handler();
	} else {
		// Do something because of message received
		msg.DLC = header.DLC;
		msg.StdID = header.StdId;
		msg.RTR = header.RTR;
		msg.IDE = header.IDE;
		msg.ExtID = header.ExtId;
		xQueueSendFromISR(xCANRxQueue, &msg, &xHigherPriorityTask);
	}
}

#ifdef __cplusplus
}
#endif