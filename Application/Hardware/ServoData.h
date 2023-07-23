#pragma once

#include "PWM.h"

namespace Application::Hardware {
  struct ServoData {
    PWM::Timer timer{};
    PWM::Channel channel{};
    struct DutyCycleRange {
      double minimum{};
      double maximum{};
    } dutyCycleRange{};
    double currentAngle{};
    double offsetAngle{};
    bool inverted{false};
  };
}// namespace Application::Hardware
