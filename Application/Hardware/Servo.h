#pragma once

#include "PWM.h"

namespace Application::Hardware {
  class Servo {
  public:
    struct DutyCycleRange {
      double minimum{};
      double maximum{};
    };

  private:
    PWM::Timer timer;
    PWM::Channel channel;
    DutyCycleRange dutyCycleRange{};

  public:
    Servo(PWM::Timer timer, PWM::Channel channel, DutyCycleRange dutyCycleRange);
    void moveTo(double angle);
  };
}// namespace Application::Hardware
