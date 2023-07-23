#pragma once

#include "OS/Task.h"
#include "ServoData.h"
namespace Application::Hardware {

  class ServoControl {
  public:
    struct ControlTaskParameters {
      ServoData &data;
      ServoControl &control;
      double anglesPerTicks{};
      double targetAngle{};
    };

  private:
    ServoData &servoData;
    ControlTaskParameters controlTaskParameters;
    OS::Task<void (*)(ControlTaskParameters &), ControlTaskParameters &> controlTask;

  public:
    explicit ServoControl(ServoData &servoData);
    void moveBy(double amount, double towards);
    void moveTo(double angle);
    void moveTo(double angle, double anglesPerSecond);
  };

}// namespace Application::Hardware
