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

namespace Application {
  template<typename TFunction, typename... TArgs>
    requires std::is_invocable_v<TFunction, TArgs &...>
  class Task {
  public:
    using tuple_type = std::tuple<TArgs &...>;
    using function_type = TFunction;

    enum class TaskState { Uninitialized, Running, Suspended };

  private:
    std::string_view taskName;
    TaskHandle_t taskHandle{nullptr};
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
    constexpr Task(std::string_view taskName, TFunction &&function, TArgs &...args)
        : taskName(taskName),
          internalParam{taskState, function, {args...}} {};
    void run() {
      if (taskHandle != nullptr && taskState != TaskState::Uninitialized) {
        vTaskDelete(taskHandle);
      }
      auto status = xTaskCreate(
              internalTaskHandler,
              taskName.data(),
              200,
              &internalParam,
              2,
              &taskHandle
      );
      configASSERT(status == pdPASS);
    }
    void suspend() {
      if (taskHandle != nullptr) vTaskSuspend(taskHandle);
      taskState = TaskState::Suspended;
    }
    void resume() const {
      if (taskHandle != nullptr) vTaskResume(taskHandle);
      taskState = TaskState::Running;
    }
  };
}// namespace Application
