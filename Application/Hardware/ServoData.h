#pragma once

#include "PWM.h"
#include "ServoTypes.h"

namespace Application::Hardware {

  template<typename T>
  concept ServoData_ = requires(T) {
    { T::timer } -> std::same_as<PWM::Timer &>;
    { T::channel } -> std::same_as<PWM::Channel &>;
    { T::currentAngle } -> std::same_as<double &>;
    { T::offsetAngle } -> std::same_as<double &>;
    { T::inverted } -> std::same_as<bool &>;
  };

  template<ServoTypes::ServoType TServoType>
  struct ServoData {
    using servo_type = TServoType;
    PWM::Timer timer{};
    PWM::Channel channel{};
    double currentAngle{};
    double offsetAngle{};
    bool inverted{false};
    ServoTypes::DutyCycleRange dutyCycleRange = TServoType::dutyCycleRange;
  };
}// namespace Application::Hardware
