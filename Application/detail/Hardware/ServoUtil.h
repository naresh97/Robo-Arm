#pragma once

namespace Application::Hardware::ServoUtil {

  template<IServoData ServoData>
  void initialize(ServoData &servoData) {
    PWM::startPWM(servoData.timer, servoData.channel);
  }

  template<IServoData ServoData>
  void moveTo(ServoData &servoData, double angle) {
    angle = angle - servoData.offsetAngle;
    if (servoData.inverted)
      angle = 180.0 - angle;
    PWM::setDutyCycle(
            servoData.timer,
            servoData.channel,
            (angle * (servoData.dutyCycleRange.maximum - servoData.dutyCycleRange.minimum) / 180.0)
                    + servoData.dutyCycleRange.minimum
    );
  }
}// namespace Application::Hardware::ServoUtil