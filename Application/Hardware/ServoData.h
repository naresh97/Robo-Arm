#pragma once

#include "PWM.h"
#include "ServoTypes.h"

namespace Application::Hardware {

  template<typename T>
  concept IServoData = requires(T) {
    { T::timer } -> std::same_as<const PWM::Timer &>;
    { T::channel } -> std::same_as<const PWM::Channel &>;
    { T::offsetAngle } -> std::same_as<const double &>;
    { T::inverted } -> std::same_as<const bool &>;
  };

  template<ServoTypes::ServoType TServoType>
  struct ServoData {
    using servo_type = TServoType;
    const PWM::Timer timer{};
    const PWM::Channel channel{};
    const double offsetAngle{};
    const bool inverted{false};
    static constexpr ServoTypes::DutyCycleRange dutyCycleRange = TServoType::dutyCycleRange;
  };
}// namespace Application::Hardware
