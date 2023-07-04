#include "Task.h"

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

#include <utility>

namespace Application {

  struct TaskImpl {
    void (*taskHandler)(void *){};
    std::string taskName{};
    TaskHandle_t taskHandle{nullptr};
    void *taskParameters{};
  };

  Task::Task(void (*taskHandler)(void *), std::string taskName, void *const taskParameters)
      : taskImpl(std::make_unique<TaskImpl>()) {
    taskImpl->taskHandler = taskHandler;
    taskImpl->taskName = std::move(taskName);
    taskImpl->taskParameters = taskParameters;
  }
  void Task::run() {
    auto status = xTaskCreate(
            taskImpl->taskHandler,
            taskImpl->taskName.c_str(),
            200,
            taskImpl->taskParameters,
            2,
            &(taskImpl->taskHandle)
    );
    configASSERT(status == pdPASS);
  }
  void Task::setTaskParameters(void *pTaskParameters) {
    taskImpl->taskParameters = pTaskParameters;
  }
  void Task::suspend() {
    if (taskImpl->taskHandle != nullptr) vTaskSuspend(taskImpl->taskHandle);
  }
  void Task::resume() {
    if (taskImpl->taskHandle != nullptr) vTaskResume(taskImpl->taskHandle);
  }
  void Task::delay(int ticks) {
    vTaskDelay(ticks);
  }
  void Task::dispose() {
    vTaskDelete(nullptr);
  }
  Task::~Task() {
    vTaskDelete(taskImpl->taskHandle);
  }
  void Task::startScheduler() {
    vTaskStartScheduler();
  }
}// namespace Application