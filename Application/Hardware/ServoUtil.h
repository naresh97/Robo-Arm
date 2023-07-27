#pragma once

#include "OS/Task.h"
#include "ServoData.h"

namespace Application::Hardware::ServoUtil {
  template<IServoData ServoData>
  void initialize(ServoData &servoData);

  template<IServoData ServoData>
  void moveTo(ServoData &servoData, double angle);

}// namespace Application::Hardware::ServoUtil

#include "../detail/Hardware/ServoUtil.h"
