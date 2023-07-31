#pragma once

#include "Hardware/ServoData.h"
#include "OS/Task.h"

#include <array>

namespace Application::Hardware {

  template<Hardware::IServoData ServoData>
  struct ServoTargeting {
    const ServoData servoData{};
    double currentPosition{90};
    double targetPosition{90};
    double anglesPerTick{};
  };

  template<typename T>
  concept IServoController = requires(T x) {
    {
      x.setTarget(std::declval<int>(), std::declval<double>(), std::declval<double>())
    } -> std::same_as<void>;
  };

  template<Hardware::IServoData... ServoData>
  class ServoController {
    static constexpr auto ServoControllerTaskStackDepth = 200;
    static constexpr auto NumberOfServos = sizeof...(ServoData);

    using ServoTargetingTuple = std::tuple<ServoTargeting<ServoData>...>;

    struct TaskParameters {
      ServoController &servoController;
      ServoTargetingTuple servoTargetingTuple;
      decltype(xTaskGetTickCount()) lastTick{};
    } taskParameters;

    static void taskHandler(TaskParameters &targetingData);
    OS::Task<ServoControllerTaskStackDepth, void (*)(TaskParameters &), TaskParameters &> task{
            taskHandler,
            taskParameters};

  public:
    explicit ServoController(ServoData... servoData);
    void run();
    auto &getTask();
    void setTarget(int index, double targetPosition, double anglesPerSecond);
  };

}// namespace Application::Hardware

#include "detail/Hardware/ServoController.h"