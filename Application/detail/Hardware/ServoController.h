#pragma once
#include "ServoUtil.h"
namespace Application::Hardware {

  template<Hardware::IServoData... ServoData>
  auto &ServoController<ServoData...>::getTask() {
    return task;
  }

  static void controlServo(auto &servoTargeting, auto &servosRequiringTargeting) {
    static constexpr auto ANGLE_TOLERANCE = 1;
    const double distance = servoTargeting.targetPosition - servoTargeting.currentPosition;
    if (std::abs(distance) <= ANGLE_TOLERANCE) {
      servosRequiringTargeting--;
      return;
    }
    servoTargeting.currentPosition += servoTargeting.anglesPerTick * distance / std::abs(distance);
    Hardware::ServoUtil::moveTo(servoTargeting.servoData, servoTargeting.currentPosition);
  }

  static void for_each(auto fn, auto &tup) {
    std::apply(
            [fn](auto &...args) {
              (fn(args), ...);
            },
            tup
    );
  }

  template<Hardware::IServoData... ServoData>
  void ServoController<ServoData...>::taskHandler(ServoController::TaskParameters &targetingData) {
    auto &task = targetingData.servoController.getTask();

    while (true) {
      auto servosRequiringTargeting = 3;
      for_each(
              [&](auto &x) {
                controlServo(x, servosRequiringTargeting);
              },
              targetingData.servoTargetingTuple
      );
      if (servosRequiringTargeting == 0)
        OS::TaskUtils::suspend();
      task.clock(1);
    }
  }

  template<Hardware::IServoData... ServoData>
  void ServoController<ServoData...>::run() {
    task.run();
  }

  static auto createTaskParameters(auto... servoData) {
    return std::tuple{ServoTargeting<decltype(servoData)>{.servoData = servoData}...};
  }

  void initializeServos(auto &...servoData) {
    (
            [](auto &x) {
              Hardware::ServoUtil::initialize(x);
            }(servoData),
            ...
    );
  }

  template<Hardware::IServoData... ServoData>
  ServoController<ServoData...>::ServoController(ServoData... servoData)
      : taskParameters{*this, createTaskParameters(servoData...)} {
    initializeServos(servoData...);
  }

  void runOnElement(int index, auto &tup, auto fn) {
    switch (index) {
      case 0:
        fn(std::get<0>(tup));
        break;
      case 1:
        fn(std::get<1>(tup));
        break;
      case 2:
        fn(std::get<2>(tup));
        break;
      default:
        throw std::logic_error("Element index not defined");
    }
  }

  template<Hardware::IServoData... ServoData>
  void ServoController<ServoData...>::setTarget(
          int index,
          double targetPosition,
          double anglesPerSecond
  ) {
    runOnElement(index, taskParameters.servoTargetingTuple, [=](auto &targeting) {
      targeting.targetPosition = targetPosition;
      targeting.anglesPerTick = anglesPerSecond / OS::TaskTickRate;
    });
    task.run();
  }

}// namespace Application::Hardware