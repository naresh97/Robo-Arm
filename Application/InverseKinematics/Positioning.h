#pragma once

#include <array>
namespace Application::Kinematics {

  static constexpr int EffectorDimensions = 3;// Planar - X,Y,A
  static constexpr int DegreesOfFreedom = 3;

  class Positioning {
    std::array<double, DegreesOfFreedom> actuatorPositions{};
    std::array<double, DegreesOfFreedom - 1> linkLengths;
    std::array<double, EffectorDimensions> desiredEffectorPosition{};

  public:
    explicit Positioning(const std::array<double, DegreesOfFreedom - 1> &linkLengths);
    
    void updateActuatorPositions(std::array<double, DegreesOfFreedom> positions);
    void updateDesiredEffectorPosition(std::array<double, EffectorDimensions> positions);
    [[nodiscard]] std::array<double, EffectorDimensions> getCurrentEffectorPosition();
    [[nodiscard]] std::array<double, DegreesOfFreedom> positionEffector();

    static void convertToWorldAngles(std::array<double, DegreesOfFreedom> &servoAngles);
    static void convertToToolAngles(std::array<double, DegreesOfFreedom> &servoAngles);
  };
}// namespace Application::Kinematics
