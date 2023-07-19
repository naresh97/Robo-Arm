#include "Servo.h"

extern "C" {
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
}

#include <algorithm>

namespace Application::Hardware {
  Servo::Servo(
          PWM::Timer timer, PWM::Channel channel, DutyCycleRange dutyCycleRange, double initialAngle
  )
      : timer(timer),
        channel(channel),
        dutyCycleRange(dutyCycleRange),
        currentAngle(initialAngle) {
    PWM::startPWM(timer, channel);
    moveTo(currentAngle);
    Task::delay(500);
  }
  void Servo::moveTo(double angle) {
    currentAngle = angle;
    angle = angle - offsetAngle;
    if (inverted) angle = 180.0 - angle;

    PWM::setDutyCycle(
            timer,
            channel,
            ((angle * (dutyCycleRange.maximum - dutyCycleRange.minimum)) / 180.0) +
                    dutyCycleRange.minimum
    );
  }
  void Servo::moveTo(double angle, double anglesPerSecond) {
    angle = std::clamp(angle, 0.0, 180.0);
    servoTaskParameters.instance = this;
    servoTaskParameters.anglesPerTicks = anglesPerSecond / configTICK_RATE_HZ;
    servoTaskParameters.targetAngle = angle;
    if (servoMovementTask != nullptr) servoMovementTask.reset();
    servoMovementTask = std::make_unique<Task>(
            [](void *p) {
              auto params = static_cast<ServoTaskParameters *>(p);
              if (params == nullptr) Task::dispose();
              while (true) {
                if (std::abs(params->instance->currentAngle - params->targetAngle) <= 1.0) break;
                params->instance->moveBy(params->anglesPerTicks, params->targetAngle);
                Task::delay(1);
              }
              Task::dispose();
            },
            "servoControlTask",
            &servoTaskParameters
    );
    servoMovementTask->run();
  }
  void Servo::moveBy(double amount, double towards) {
    if (currentAngle == towards) return;
    int direction = 1;
    if (towards < currentAngle) direction = -1;
    this->moveTo(currentAngle + direction * amount);
  }
  double Servo::getCurrentAngle() const {
    return currentAngle;
  }
  void Servo::setOffset(double offset) {
    this->offsetAngle = offset;
  }
  void Servo::setInverted(bool inverted) {
    this->inverted = inverted;
  }
}// namespace Application::Hardware