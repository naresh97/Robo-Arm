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
    using tuple_type = std::tuple<TArgs &...>;
    using function_type = TFunction;

    std::string_view taskName;
    TaskHandle_t taskHandle{nullptr};

    struct InternalParam {
      function_type function;
      tuple_type args;
    } internalParam;

    void (*internalTaskHandler)(void *) = [](void *params) {
      if (params == nullptr) {
        TaskUtils::dispose();
        return;
      }
      auto &parameter = *static_cast<InternalParam *>(params);
      std::apply(parameter.function, parameter.args);
      TaskUtils::dispose();
    };

  public:
    constexpr Task(std::string_view taskName, TFunction &&function, TArgs &...args)
        : taskName(taskName),
          internalParam{function, {args...}} {};
    void run() {
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
    }
    void resume() const {
      if (taskHandle != nullptr) vTaskResume(taskHandle);
    }
    ~Task() {
      if (taskHandle != nullptr) {
        auto state = eTaskGetState(taskHandle);
        if (state != eDeleted) vTaskDelete(taskHandle);
      }
    }
  };
}// namespace Application
