#pragma once

#include <array>
namespace Application::Kinematics::Positioning {
  static constexpr int EffectorDimensions = 3;// Planar - X,Y,A
  static constexpr int DegreesOfFreedom = 3;
  using ActuatorPositionArray = std::array<double, DegreesOfFreedom>;
  using LinkLengthsArray = std::array<double, DegreesOfFreedom - 1>;
  using EffectorPositionArray = std::array<double, EffectorDimensions>;

  std::array<double, EffectorDimensions> getEffectorPosition(
          LinkLengthsArray &linkLengths, ActuatorPositionArray &actuatorPositions
  );
  std::array<double, DegreesOfFreedom> positionEffector(
          LinkLengthsArray &linkLengths, EffectorPositionArray &desiredEffectorPosition
  );
  void convertToWorldAngles(std::array<double, DegreesOfFreedom> &servoAngles);
  void convertToToolAngles(std::array<double, DegreesOfFreedom> &servoAngles);
}// namespace Application::Kinematics::Positioning
