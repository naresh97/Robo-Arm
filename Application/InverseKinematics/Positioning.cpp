#include "Positioning.h"

#include <algorithm>
#include <cmath>

namespace Application::Kinematics::Positioning2D {

  static double toDegrees(double radians) {
    return radians * 180 / M_PI;
  }

  std::array<double, DegreesOfFreedom> positionEffector(
          LinkLengthsArray &linkLengths,
          EffectorPositionArray &desiredEffectorPosition
  ) {
    std::array<double, DegreesOfFreedom> angles{};

    auto q2 = (std::pow(desiredEffectorPosition[0], 2) + std::pow(desiredEffectorPosition[1], 2)
               - std::pow(linkLengths[0], 2) - std::pow(linkLengths[1], 2))
              / (2 * linkLengths[0] * linkLengths[1]);
    q2 = std::clamp(q2, -1.0, 1.0);
    q2 = std::acos(q2);

    auto q1 = std::atan2(desiredEffectorPosition[1], desiredEffectorPosition[0])
              - std::atan2(
                      linkLengths[0] * std::sin(q2),
                      linkLengths[0] + linkLengths[1] * std::cos(q2)
              );

    auto q3 = desiredEffectorPosition[2] - q1 - q2;

    angles[0] = toDegrees(q1);
    angles[1] = toDegrees(q2);
    angles[2] = toDegrees(q3);

    return angles;
  }

  std::array<double, EffectorDimensions> getEffectorPosition(
          LinkLengthsArray &linkLengths,
          ActuatorPositionArray &actuatorPositions
  ) {
    std::array<double, EffectorDimensions> position{};
    auto phi = actuatorPositions[0] + actuatorPositions[1] + actuatorPositions[2];
    auto x = linkLengths[0] * std::cos(actuatorPositions[0] + actuatorPositions[1])
             + linkLengths[1] * std::cos(linkLengths[0]);
    auto y = linkLengths[0] * std::sin(actuatorPositions[0] + actuatorPositions[1])
             + linkLengths[1] * std::sin(linkLengths[0]);
    position[0] = x;
    position[1] = y;
    position[2] = phi;
    return position;
  }

  static double remap(double value, double low1, double high1, double low2, double high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
  }

  void convertToWorldAngles(std::array<double, DegreesOfFreedom> &servoAngles) {
    servoAngles[0] = remap(servoAngles[0], 0, 180, 180, 0);
    servoAngles[1] = remap(servoAngles[1], -90, 90, 180, 0);
    servoAngles[2] = remap(servoAngles[2], -90, 90, 180, 0);
  }
  void convertToToolAngles(std::array<double, DegreesOfFreedom> &servoAngles) {
    servoAngles[0] = remap(servoAngles[0], 180, 0, 0, 180);
    servoAngles[1] = remap(servoAngles[1], 180, 0, -90, 90);
    servoAngles[2] = remap(servoAngles[2], 180, 0, -90, 90);
  }

}// namespace Application::Kinematics::Positioning2D