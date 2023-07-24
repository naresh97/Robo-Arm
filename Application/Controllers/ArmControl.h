#pragma once

#include "ArmData.h"
#include <type_traits>

namespace Application::Controllers {
  auto createServoControls(auto &servoData);

  template<typename TArmData>
  class ArmControl {
    TArmData armData;

    using servo_control_type =
            decltype(createServoControls(std::declval<typename TArmData::servo_data_types &>()));
    servo_control_type servoControls;

  public:
    explicit ArmControl(TArmData armData);
    void positionTo(std::array<double, 3> targetPosition);
  };
}// namespace Application::Controllers

#include "../detail/Controllers/ArmControl.h"