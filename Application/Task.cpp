#include "Task.h"

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

#include <utility>

namespace Application {

  Task::Task(void (*taskHandler)(void *), std::string taskName, void *const taskParameters)
      : taskHandler(taskHandler),
        taskName(std::move(taskName)),
        taskParameters(taskParameters) {
  }

  void Task::run() {
    auto status = xTaskCreate(
            taskHandler,
            taskName.c_str(),
            200,
            taskParameters,
            2,
            reinterpret_cast<TaskHandle_t *>(&taskHandle)
    );
    configASSERT(status == pdPASS);
  }
  void Task::setTaskParameters(void *pTaskParameters) {
    taskParameters = pTaskParameters;
  }
  void Task::suspend() {
    if (taskHandle != nullptr) vTaskSuspend(static_cast<TaskHandle_t>(taskHandle));
  }
  void Task::resume() {
    if ((TaskHandle_t) taskHandle != nullptr) vTaskResume(static_cast<TaskHandle_t>(taskHandle));
  }
  void Task::delay(int ticks) {
    vTaskDelay(ticks);
  }
  void Task::dispose() {
    vTaskDelete(nullptr);
  }
  Task::~Task() {
    if ((TaskHandle_t) taskHandle != nullptr) {
      auto state = eTaskGetState(static_cast<TaskHandle_t>(taskHandle));
      if (state != eDeleted) vTaskDelete(static_cast<TaskHandle_t>(taskHandle));
    }
  }
  void Task::startScheduler() {
    vTaskStartScheduler();
  }
}// namespace Application