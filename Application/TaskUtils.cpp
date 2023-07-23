#include "TaskUtils.h"

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace Application::TaskUtils {
  void dispose() {
    vTaskDelete(nullptr);
  }
  void delay(int ticks) {
    vTaskDelay(ticks);
  }
  void startScheduler() {
    vTaskStartScheduler();
  }
}// namespace Application::TaskUtils