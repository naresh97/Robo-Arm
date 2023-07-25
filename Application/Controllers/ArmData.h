#pragma once

#include "../Hardware/ServoData.h"
#include "Hardware/ServoControl.h"
#include "InverseKinematics/Positioning.h"

#include <tuple>

namespace Application::Controllers {
  template<Hardware::ServoData_... TServoDatas>
  struct ArmData {
    using servo_data_types = std::tuple<TServoDatas...>;
    std::tuple<TServoDatas...> servoData;
    Kinematics::Positioning2D::LinkLengthsArray linkLengths;
    explicit ArmData(
            Kinematics::Positioning2D::LinkLengthsArray linkLengths,
            TServoDatas... servoDatas
    )
        : servoData{std::move(servoDatas)...},
          linkLengths(linkLengths){};
  };
}// namespace Application::Controllers
