#include "Servo.h"

namespace Application::Hardware {
  Servo::Servo(PWM::Timer timer, PWM::Channel channel, DutyCycleRange dutyCycleRange)
      : timer(timer),
        channel(channel),
        dutyCycleRange(dutyCycleRange) {
    PWM::startPWM(timer, channel);
  }
  void Servo::moveTo(double angle) {
    PWM::setDutyCycle(
            timer,
            channel,
            ((angle * (dutyCycleRange.maximum - dutyCycleRange.minimum)) / 180.0) +
                    dutyCycleRange.minimum
    );
  }
}// namespace Application::Hardware