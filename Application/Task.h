#pragma once

#include <any>
#include <memory>
#include <string>

namespace Application {
  class Task {
    void (*taskHandler)(void *){};
    std::string taskName{};
    void *taskHandle{nullptr};
    void *taskParameters{};

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
