#pragma once

#include "PWM.h"

namespace Application::Hardware {
  class Servo {
    PWM::Timer timer;
    PWM::Channel channel;
    double minimumDutyCycle{};
    double maximumDutyCycle{};

  public:
    Servo(PWM::Timer timer, PWM::Channel channel);
    void setDutyCycleRange(double minimum, double maximum);
    void moveTo(double angle);
  };
}// namespace Application::Hardware
