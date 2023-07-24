#pragma once

namespace Application::Hardware {

  template<ServoTypes::ServoType TServoType>
  void ServoControl<TServoType>::servoControlTask(
          ServoControl::ControlTaskParameters &controlTaskParameters
  ) {
    {
      while (true) {
        if (std::abs(controlTaskParameters.data.currentAngle - controlTaskParameters.targetAngle) <=
            1.0)
          OS::TaskUtils::suspend();
        controlTaskParameters.control.moveBy(
                controlTaskParameters.anglesPerTicks,
                controlTaskParameters.targetAngle
        );
        OS::TaskUtils::delay(1);
      }
    }
  }

  template<ServoTypes::ServoType TServoType>
  ServoControl<TServoType>::ServoControl(ServoData<TServoType> servoData)
      : servoData(std::move(servoData)),
        controlTaskParameters(servoData, *this),
        controlTask{servoControlTask, controlTaskParameters} {
    PWM::startPWM(servoData.timer, servoData.channel);
    moveTo(servoData.currentAngle);
    OS::TaskUtils::delay(180. / 20.);
  }

  template<ServoTypes::ServoType TServoType>
  void ServoControl<TServoType>::moveBy(double amount, double towards) {
    if (servoData.currentAngle == towards) return;
    int direction = 1;
    if (towards < servoData.currentAngle) direction = -1;
    moveTo(servoData.currentAngle + direction * amount);
  }

  template<ServoTypes::ServoType TServoType>
  void ServoControl<TServoType>::moveTo(double angle) {
    servoData.currentAngle = angle;
    angle = angle - servoData.offsetAngle;
    if (servoData.inverted) angle = 180.0 - angle;
    PWM::setDutyCycle(
            servoData.timer,
            servoData.channel,
            ((angle * (servoData.dutyCycleRange.maximum - servoData.dutyCycleRange.minimum)) / 180.0
            ) + servoData.dutyCycleRange.minimum
    );
  }

  template<ServoTypes::ServoType TServoType>
  void ServoControl<TServoType>::moveTo(double angle, double anglesPerSecond) {
    angle = std::clamp(angle, 0.0, 180.0);
    controlTaskParameters.anglesPerTicks = anglesPerSecond / configTICK_RATE_HZ;
    controlTaskParameters.targetAngle = angle;
    controlTask.resume();
  }
}// namespace Application::Hardware