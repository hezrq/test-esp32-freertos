#include <stdio.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include "driver/ledc.h"
#include "esp_err.h"

int app_main(void) {
  /*
   * Prepare and set configuration of timers
   * that will be used by LED Controller
   */
  ledc_timer_config_t ledc_timer = {
      .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
      .freq_hz = 5000,                      // frequency of PWM signal
      .speed_mode = LEDC_LOW_SPEED_MODE,           // timer mode
      .timer_num = LEDC_TIMER_0// timer index
  };

  // Set configuration of timer0 for high speed channels
  ledc_timer_config(&ledc_timer);

  /*
   * Prepare individual configuration
   * for each channel of LED Controller
   * by selecting:
   * - controller's channel number
   * - output duty cycle, set initially to 0
   * - GPIO number where LED is connected to
   * - speed mode, either high or low
   * - timer servicing selected channel
   *   Note: if different channels use one timer,
   *         then frequency and bit_num of these channels
   *         will be the same
   */
  ledc_channel_config_t ledc_channel = {
      .channel = LEDC_CHANNEL_0,
      .duty = 0,
      .gpio_num = 4,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .hpoint = 0,
      .timer_sel = LEDC_TIMER_0,
  };

  ledc_channel_config(&ledc_channel);

  while (true) {
    for (int duty = 0; duty <= 4000; duty += 10) {
      printf("0LED %d\n", duty);
      ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
      ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    for (int duty = 4000; duty >= 0; duty -= 10) {
      printf("1LED %d\n", duty);
      ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
      ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
  }

  return 0;
}

extern void esp_vApplicationTickHook();
void IRAM_ATTR vApplicationTickHook() { esp_vApplicationTickHook(); }

extern void esp_vApplicationIdleHook();
void vApplicationIdleHook() { esp_vApplicationIdleHook(); }

void vApplicationDaemonTaskStartupHook(void) {}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
  /* If the buffers to be provided to the Idle task are declared inside this
   * function then they must be declared static - otherwise they will be
   * allocated on the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[configIDLE_TASK_STACK_SIZE];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle
   * task's state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
   * Note that, as the array is necessarily of type StackType_t,
   * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configIDLE_TASK_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetTimerTaskMemory() to provide the memory that
 * is used by the RTOS daemon/time task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
  /* If the buffers to be provided to the Timer task are declared inside this
   * function then they must be declared static - otherwise they will be
   * allocated on the stack and so not exists after this function exits. */
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle
   * task's state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  /* Pass out the array that will be used as the Timer task's stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
   * Note that, as the array is necessarily of type StackType_t,
   * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
