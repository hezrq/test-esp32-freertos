#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

void app_main(void) {
  gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

  int count = 0;
  while (true) {
    printf("LED %d\n", count % 2);
    gpio_set_level(GPIO_NUM_4, count % 2);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    count++;
  }
}

extern void esp_vApplicationTickHook();
void IRAM_ATTR vApplicationTickHook() { esp_vApplicationTickHook(); }

extern void esp_vApplicationIdleHook();
void vApplicationIdleHook() { esp_vApplicationIdleHook(); }

void vApplicationDaemonTaskStartupHook(void) {}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[configIDLE_TASK_STACK_SIZE];

  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  *pulIdleTaskStackSize = configIDLE_TASK_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
