#pragma once

#include <cstdint>

namespace Application::Hardware::PWM {
  enum class Timer { Timer2 };
  enum class Channel { Channel1, Channel2, Channel3 };
  void startPWM(Timer timer, Channel channel);
  void setDutyCycle(Timer timer, Channel channel, double dutyCycle);
}// namespace Application::Hardware::PWM
