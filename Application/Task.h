#pragma once

#include <memory>
#include <string>

namespace Application {
  class TaskImpl;
  class Task {
    std::unique_ptr<TaskImpl> taskImpl;

  public:
    Task(void (*taskHandler)(void *), std::string taskName, void *taskParameters = nullptr);
    ~Task();
    void setTaskParameters(void *pTaskParameters);
    void run();
    void suspend();
    void resume();
    static void delay(int ticks);
    static void dispose();
    static void startScheduler();
  };
}// namespace Application
