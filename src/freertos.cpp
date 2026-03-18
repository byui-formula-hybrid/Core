#include "main.h"
#include "shared_variables.h"

// ADD Priority description

void SampleDummyTask(void *argument);
void CANConsumerTask(void *argument);
void CANProducerTask(void *argument);
void PumpPWMTask(void *argument);
void HeartbeatTask(void *argument);
void StartTask02(void *argument);

void FREERTOS_TASK_Init(void);

/**
  * @brief  FreeRTOS initialization  adding tasks
  * @param  None
  * @retval None
  */
void FREERTOS_TASK_Init(void) {
  // create tasks
  xTaskCreate(SampleDummyTask, "Dummy", 256, NULL, 10, NULL);
  xTaskCreate(CANConsumerTask, "CAN", 1024 / sizeof(StackType_t), NULL, 1, NULL);
  vTaskStartScheduler();
}

/**
  * @brief  Function implementing a simple sample task thread.
  * @param  argument: The pvParameters from the xTaskCreate
  * @retval None
  */
void SampleDummyTask(void *argument) {
  TickType_t temp = xTaskGetTickCount();
  int counter = 0;

  for(;;) {
    // HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
    // vTaskDelay(pdMS_TO_TICKS(500));
    // CycleLEDs(); // This LED cycler was originally for debugging and was specific to an STM32 board, the function has since been removed.
    vTaskDelayUntil(&temp, pdMS_TO_TICKS(500));
    counter++;
    printf("Counter: %d\n", counter);
  }
}

/**
* @brief Function implementing the CANConsumer thread. This is a very simple reading the CAN message
* @param argument: Not used
* @retval None
*/
void CANConsumerTask(void *argument) {
  CAN_Msg_t msg;
  for(;;) {
    if (xQueueReceive(xCANRxQueue, &msg, portMAX_DELAY) == pdPASS) {
      // process the can message here...

      // Simple example for printing the messages (slow), better to send to a function to process the CAN messages:
      printf("ID: 0x%x MSG: ", msg.StdID);
      for (int i = 0; i < msg.DLC; i++) {
        if (msg.Data[i] < 0x10) printf("0");
        printf("%x", msg.Data[i]);
        printf(" ");
      }
      printf("\n");
    }
  }
}

/**
* @brief Sends CAN messages to the CAN bus.
* @param argument: Not used
* @retval None
*/
void CANProducerTask(void *argument) {
  CAN_Msg_t msg;
  uint32_t mailbox;

  // !! == EXAMPLE for adding a CAN message to the queue be sent from other tasks == !!
  // CAN_Msg_t myMsg;
  // build up the myMsg (ie. data, DLC, etc...)
  // xQueueSend(xCANTxQueue, &myMsg, 0);
  // !! == EXAMPLE for adding a CAN message to the queue be sent from other tasks == !!

  for (;;) {
    if (xQueueReceive(xCANTxQueue, &msg, portMAX_DELAY) == pdPASS) {
      // call the hardware specific method for sending a CAN message
    }
  }
}

/**
* @brief General heartbeat task set up to run every half second.
* @param argument: Not used
* @retval None
*/
void HeartbeatTask(void *argument) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(500);

  for(;;) {
    // do heart beat stuff.....
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

/**
* @brief Pump PWM task.
* @param argument: Not used
* @retval None
*/
void PumpPWMTask(void *argument) {
  uint32_t pulseWidth = 0;

  for (;;) {
    // send PWM signal
  }
}

/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
void StartTask02(void *argument) {
}