#include "PWM.h"

extern "C" {
#include "stm32f4xx_hal.h"
}

extern TIM_HandleTypeDef htim2;

namespace Application::Hardware::PWM {
  static auto &getTimer(Timer timer) {
    switch (timer) {
      default:
      case Timer::Timer2:
        return htim2;
    }
  }
  static auto getChannel(Channel channel) {
    switch (channel) {
      default:
      case Channel::Channel1:
        return TIM_CHANNEL_1;
      case Channel::Channel2:
        return TIM_CHANNEL_2;
      case Channel::Channel3:
        return TIM_CHANNEL_3;
    }
  }

  static uint32_t getDutyCycle(double dutyCycle) {
    return static_cast<uint32_t>(dutyCycle * static_cast<double>(INT32_MAX));
  }

  void startPWM(Timer timer, Channel channel) {
    HAL_TIM_PWM_Start(&getTimer(timer), getChannel(channel));
  }

  void setDutyCycle(Timer timer, Channel channel, double dutyCycle) {
    __HAL_TIM_SET_COMPARE(
            &getTimer(timer),
            getChannel(channel),
            getTimer(timer).Init.Period * dutyCycle
    );
  }

}// namespace Application::Hardware::PWM