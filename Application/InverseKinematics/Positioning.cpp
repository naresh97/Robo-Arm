#include "Positioning.h"

#include <algorithm>
#include <cmath>

namespace Application::Kinematics {
  std::array<double, DegreesOfFreedom> Positioning::positionEffector() {
    std::array<double, DegreesOfFreedom> angles{};

    auto q2 = (std::pow(desiredEffectorPosition[0], 2) + std::pow(desiredEffectorPosition[1], 2) -
               std::pow(linkLengths[0], 2) - std::pow(linkLengths[1], 2)) /
              (2 * linkLengths[0] * linkLengths[1]);
    q2 = std::clamp(q2, -1.0, 1.0);
    q2 = std::acos(q2);

    auto q1 = std::atan2(desiredEffectorPosition[1], desiredEffectorPosition[0]) -
              std::atan2(
                      linkLengths[0] * std::sin(q2),
                      linkLengths[0] + linkLengths[1] * std::cos(q2)
              );

    auto q3 = desiredEffectorPosition[2] - q1 - q2;

    angles[0] = q1;
    angles[1] = q2;
    angles[2] = q3;

    return angles;
  }

  std::array<double, EffectorDimensions> Positioning::getCurrentEffectorPosition() {
    std::array<double, EffectorDimensions> position{};
    auto phi = actuatorPositions[0] + actuatorPositions[1] + actuatorPositions[2];
    auto x = linkLengths[0] * std::cos(actuatorPositions[0] + actuatorPositions[1]) +
             linkLengths[1] * std::cos(linkLengths[0]);
    auto y = linkLengths[0] * std::sin(actuatorPositions[0] + actuatorPositions[1]) +
             linkLengths[1] * std::sin(linkLengths[0]);
    position[0] = x;
    position[1] = y;
    position[2] = phi;
    return position;
  }

  void Positioning::updateActuatorPositions(std::array<double, DegreesOfFreedom> positions) {
    this->actuatorPositions = positions;
  }
  void Positioning::updateDesiredEffectorPosition(std::array<double, EffectorDimensions> positions
  ) {
    this->desiredEffectorPosition = positions;
  }

}// namespace Application::Kinematics