#include "Task.h"

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
            this->taskName.c_str(),
            200,
            taskParameters,
            2,
            &(this->taskHandle)
    );
    configASSERT(status == pdPASS);
  }
  void Task::setTaskParameters(void *pTaskParameters) {
    this->taskParameters = pTaskParameters;
  }
  void Task::dispose() {
    if (this->taskHandle != nullptr) vTaskDelete(this->taskHandle);
  }
  void Task::suspend() {
    if (this->taskHandle != nullptr) vTaskSuspend(this->taskHandle);
  }
  void Task::resume() {
    if (this->taskHandle != nullptr) vTaskResume(this->taskHandle);
  }
  Task::~Task() {
    this->dispose();
  }
}// namespace Application