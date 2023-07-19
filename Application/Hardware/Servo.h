#pragma once

#include "PWM.h"
#include "Task.h"

#include <memory>

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
    std::unique_ptr<Task> servoMovementTask{};
    struct ServoTaskParameters {
      Servo *instance{};
      double anglesPerTicks{};
      double targetAngle{};
    } servoTaskParameters{};
    double currentAngle{};
    double offsetAngle{};
    bool inverted{false};

  public:
    Servo(PWM::Timer timer, PWM::Channel channel, DutyCycleRange dutyCycleRange,
          double initialAngle = 0);
    void setOffset(double offset);
    void setInverted(bool inverted);
    void moveBy(double amount, double towards);
    void moveTo(double angle);
    void moveTo(double angle, double anglesPerSecond);
    [[nodiscard]] double getCurrentAngle() const;
  };
}// namespace Application::Hardware
