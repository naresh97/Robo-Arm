#include "TaskUtils.h"

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace Application::OS::TaskUtils {
  void dispose() {
    vTaskDelete(nullptr);
  }
  void delay(int ticks) {
    vTaskDelay(ticks);
  }
  void startScheduler() {
    vTaskStartScheduler();
  }
  void suspend() {
    vTaskSuspend(nullptr);
  }
}// namespace Application::OS::TaskUtils