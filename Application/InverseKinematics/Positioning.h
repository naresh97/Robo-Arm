#pragma once

#include <array>
namespace Application::Kinematics {

  static constexpr int EffectorDimensions = 3;// Planar - X,Y,A
  static constexpr int DegreesOfFreedom = 3;

  class Positioning {
    std::array<double, DegreesOfFreedom> actuatorPositions{};
    std::array<double, DegreesOfFreedom - 1> linkLengths{};
    std::array<double, EffectorDimensions> desiredEffectorPosition{};

  public:
    void updateActuatorPositions(std::array<double, DegreesOfFreedom> positions);
    void updateDesiredEffectorPosition(std::array<double, EffectorDimensions> positions);
    std::array<double, EffectorDimensions> getCurrentEffectorPosition();
    std::array<double, DegreesOfFreedom> positionEffector();
  };
}// namespace Application::Kinematics
