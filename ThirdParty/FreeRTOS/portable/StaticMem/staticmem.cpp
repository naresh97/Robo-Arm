extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
}
#include <array>

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
  static constexpr auto TimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  static StaticTask_t TimerTaskControlBlock{};
  static std::array<StackType_t, TimerTaskStackSize> TimerTaskStack{};
  *ppxTimerTaskTCBBuffer = &TimerTaskControlBlock;
  *ppxTimerTaskStackBuffer = &TimerTaskStack[0];
  *pulTimerTaskStackSize = TimerTaskStackSize;
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
  static constexpr auto IdleTaskStackSize = configMINIMAL_STACK_SIZE;
  static StaticTask_t IdleTaskControlBlock{};
  static std::array<StackType_t, IdleTaskStackSize> IdleTaskStack{};

  *ppxIdleTaskTCBBuffer = &IdleTaskControlBlock;
  *ppxIdleTaskStackBuffer = &IdleTaskStack[0];
  *pulIdleTaskStackSize = IdleTaskStackSize;
}