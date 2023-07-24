#include "Application.h"

#include "Controllers/ArmControl.h"
#include "Controllers/ArmData.h"
#include "Hardware/ServoControl.h"
#include "Hardware/ServoData.h"
#include "InverseKinematics/Positioning.h"
#include "OS/Task.h"

void Application_RunApplication() {
  using namespace Application;
  using namespace Application::OS;
  using namespace Application::Hardware;
  using namespace Application::Kinematics;
  using namespace Application::Controllers;

  static auto task1 = Task{[]() {
    auto servo1 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel2, 90, -7};
    auto servo2 =
            ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel1, 90, -10, true};
    auto servo3 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel3, 90, 5};
    auto linkLengths = Kinematics::Positioning2D::LinkLengthsArray{100, 100};
    auto armData = ArmData{linkLengths, servo1, servo2, servo3};

    static auto armControl = ArmControl{armData};
    TaskUtils::delay(2000);
    double c = 0;
    double i = 5;
    while (true) {
      c += i;
      if (c > 50 || c < -50) { i = -i; }
      auto targetPosition = std::array{c, 150., 3.14 / 2.};
      armControl.positionTo(targetPosition);
      TaskUtils::delay(500);
    }
  }};
  task1.run();
  TaskUtils::startScheduler();
}
