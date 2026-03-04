#include "main.h"
#include "shared_variables.h"

// ADD Priority description

void SampleDummyTask(void *argument);
void CANConsumer(void *argument);
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
  xTaskCreate(CANConsumer, "CAN", 1024 / sizeof(StackType_t), NULL, 1, NULL);
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
void CANConsumer(void *argument) {
  for(;;) {
    if (msg_received > 0) {
      printf("ID: 0x%x MSG: ", headerFIFO0.StdId);
      for (int i = 0; i < headerFIFO0.DLC; i++) {
        if (dataFIFO0[i] < 0x10) printf("0");
        printf("%x", dataFIFO0[i]);
        printf(" ");
      }
      printf("\n");
      msg_received = 0;
    }
  }
}

/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
void StartTask02(void *argument) {
}