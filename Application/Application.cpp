#include "Application.h"

#include "Hardware/ServoControl.h"
#include "Hardware/ServoData.h"
#include "InverseKinematics/Positioning.h"
#include "OS/Task.h"

void Application_RunApplication() {
  using namespace Application;
  using namespace Application::OS;
  using namespace Application::Hardware;
  using namespace Application::Kinematics;

  static auto task1 = Task{[]() {
    auto dutyCycleRange = ServoData::DutyCycleRange{0.43 / 20.0, 2.4 / 20.0};
    static auto servo1 =
            ServoData{PWM::Timer::Timer2, PWM::Channel::Channel2, dutyCycleRange, 90, -7};
    static auto servo2 =
            ServoData{PWM::Timer::Timer2, PWM::Channel::Channel1, dutyCycleRange, 90, -10, true};
    static auto servo3 =
            ServoData{PWM::Timer::Timer2, PWM::Channel::Channel3, dutyCycleRange, 90, 5};
    static auto servo1control = ServoControl{servo1};
    static auto servo2control = ServoControl{servo2};
    static auto servo3control = ServoControl{servo3};
    Kinematics::Positioning2D::LinkLengthsArray linkLengths{100, 100};
    TaskUtils::delay(2000);

    double c = 0;
    double i = 5;

    while (true) {
      c += i;
      if (c > 50 || c < -50) { i = -i; }
      auto x = std::array{servo1.currentAngle, servo2.currentAngle, servo3.currentAngle};
      Kinematics::Positioning2D::convertToWorldAngles(x);
      auto targetPosition = std::array{c, 150., 3.14 / 2.};
      auto newX = Positioning2D::positionEffector(linkLengths, targetPosition);
      Kinematics::Positioning2D::convertToToolAngles(newX);
      servo1control.moveTo(newX[0], 45);
      servo2control.moveTo(newX[1], 45);
      servo3control.moveTo(newX[2], 45);
      TaskUtils::delay(500);
    }
  }};
  task1.run();
  TaskUtils::startScheduler();
}
