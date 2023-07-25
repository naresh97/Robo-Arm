#pragma once

namespace Application::Controllers {
  auto createServoControls(auto &servoData) {
    return std::apply(
            [](auto &...datas) {
              return std::tuple{Hardware::ServoControl{datas}...};
            },
            servoData
    );
  }
  template<typename TArmData>
  ArmControl<TArmData>::ArmControl(TArmData armData)
      : armData(std::move(armData)),
        servoControls(createServoControls(this->armData.servoData)) {
  }
  template<typename TArmData>
  void ArmControl<TArmData>::positionTo(std::array<double, 3> targetPosition) {
    using namespace Application::Kinematics::Positioning2D;
    auto newPosition = positionEffector(armData.linkLengths, targetPosition);
    convertToToolAngles(newPosition);

    std::get<0>(servoControls).moveTo(newPosition[0], 45);
    std::get<1>(servoControls).moveTo(newPosition[1], 45);
    std::get<2>(servoControls).moveTo(newPosition[2], 45);
  }
}// namespace Application::Controllers