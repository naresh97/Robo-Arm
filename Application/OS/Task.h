#pragma once

#include "TaskUtils.h"
#include <string_view>

#include <memory>
#include <tuple>
#include <variant>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace Application::OS {
  template<typename TFunction, typename... TArgs>
    requires std::is_invocable_v<TFunction, TArgs &...>
  class Task {
    static constexpr auto TaskStackSize = 200;

  public:
    using tuple_type = std::tuple<TArgs &...>;
    using function_type = TFunction;

    enum class TaskState { Uninitialized, Running, Suspended };

  private:
    StaticTask_t taskControlBlock{};
    TaskHandle_t taskHandle{nullptr};
    std::array<StackType_t, TaskStackSize> taskStack{};
    TaskState taskState{TaskState::Uninitialized};

    struct InternalParam {
      TaskState &state;
      function_type function;
      tuple_type args;
    } internalParam;

    void (*internalTaskHandler)(void *) = [](void *params) {
      if (params == nullptr) {
        TaskUtils::dispose();
        return;
      }
      auto &parameter = *static_cast<InternalParam *>(params);
      parameter.state = TaskState::Running;
      std::apply(parameter.function, parameter.args);
      parameter.state = TaskState::Uninitialized;
      TaskUtils::dispose();
    };

  public:
    constexpr Task(TFunction &&function, TArgs &...args)
        : internalParam{taskState, function, {args...}} {};
    void run() {
      if (taskHandle != nullptr && taskState != TaskState::Uninitialized) {
        vTaskDelete(taskHandle);
      }
      taskHandle = xTaskCreateStatic(
              internalTaskHandler,
              "",
              TaskStackSize,
              &internalParam,
              2,
              &taskStack[0],
              &taskControlBlock
      );
      configASSERT(taskHandle != nullptr);
    }
    void suspend() {
      if (taskHandle != nullptr) vTaskSuspend(taskHandle);
      taskState = TaskState::Suspended;
    }
    void resume() {
      if (taskHandle == nullptr || taskState == TaskState::Uninitialized) {
        run();
      } else {
        vTaskResume(taskHandle);
        taskState = TaskState::Running;
      }
    }
  };
}// namespace Application::OS
