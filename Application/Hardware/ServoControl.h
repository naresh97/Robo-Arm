#pragma once

#include "OS/Task.h"
#include "ServoData.h"

namespace Application::Hardware {

  template<ServoTypes::ServoType TServoType>
  class ServoControl {
  public:
    struct ControlTaskParameters {
      ServoData<TServoType> &data;
      ServoControl &control;
      double anglesPerTicks{};
      double targetAngle{};
    };

  private:
    ServoData<TServoType> servoData;
    ControlTaskParameters controlTaskParameters;
    OS::Task<void (*)(ControlTaskParameters &), ControlTaskParameters &> controlTask;
    static void servoControlTask(ControlTaskParameters &controlTaskParameters);

  public:
    explicit ServoControl(ServoData<TServoType> servoData);
    void moveBy(double amount, double towards);
    void moveTo(double angle);
    void moveTo(double angle, double anglesPerSecond);
  };

}// namespace Application::Hardware

#include "../detail/Hardware/ServoControl.h"
