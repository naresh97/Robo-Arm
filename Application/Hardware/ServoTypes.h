#pragma once

#include <concepts>

namespace Application::Hardware::ServoTypes {

  struct DutyCycleRange {
    double minimum{};
    double maximum{};
  };

  template<typename T>
  concept ServoType = requires(T x) {
    { x.dutyCycleRange } -> std::same_as<const DutyCycleRange &>;
  };

  struct SG90 {
    static constexpr DutyCycleRange dutyCycleRange{0.43 / 20.0, 2.4 / 20.0};
  };

}// namespace Application::Hardware::ServoTypes