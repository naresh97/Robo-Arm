#include "Servo.h"

namespace Application::Hardware {
  Servo::Servo(PWM::Timer timer, PWM::Channel channel)
      : timer(timer),
        channel(channel) {
    PWM::startPWM(timer, channel);
  }
  void Servo::setDutyCycleRange(double minimum, double maximum) {
    this->minimumDutyCycle = minimum;
    this->maximumDutyCycle = maximum;
  }
  void Servo::moveTo(double angle) {
    PWM::setDutyCycle(
            timer,
            channel,
            ((angle * (maximumDutyCycle - minimumDutyCycle)) / 180.0) + minimumDutyCycle
    );
  }
}// namespace Application::Hardware