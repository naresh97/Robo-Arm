#pragma once

#include <string>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace Application {
  class Task {
    void (*taskHandler)(void *);
    const std::string taskName;
    TaskHandle_t taskHandle{nullptr};
    void *taskParameters;

  public:
    Task(void (*taskHandler)(void *), std::string taskName, void *taskParameters = nullptr);
    ~Task();
    void setTaskParameters(void *pTaskParameters);
    void run();
    void dispose();
    void suspend();
    void resume();
  };
}// namespace Application
