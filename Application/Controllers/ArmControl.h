#pragma once

#include "Hardware/ServoController.h"
#include "InverseKinematics/Positioning.h"

namespace Application::Controllers {

  template<Hardware::IServoController ServoController>
  class ArmControl {
    Kinematics::Positioning2D::LinkLengthsArray linkLengths;
    ServoController &servoController;

  public:
    explicit ArmControl(
            ServoController &servoController,
            Kinematics::Positioning2D::LinkLengthsArray linkLengthsArray
    )
        : linkLengths(linkLengthsArray),
          servoController(servoController) {}

    void positionTo(std::array<double, 3> targetPosition) {
      using namespace Application::Kinematics::Positioning2D;
      auto newPosition = positionEffector(linkLengths, targetPosition);
      convertToToolAngles(newPosition);
      for (int i = 0; i < newPosition.size(); ++i)
        servoController.setTarget(i, newPosition[i], 360);
    }
  };

}// namespace Application::Controllers
