#include "Application.h"

#include "Controllers/ArmControl.h"
#include "Hardware/ServoController.h"
#include "Hardware/ServoData.h"
#include "OS/Task.h"

void Application_RunApplication() {
  using namespace Application;
  using namespace Application::OS;
  using namespace Application::Hardware;
  using namespace Application::Kinematics;
  using namespace Application::Controllers;

  auto servo1 = ServoData<ServoTypes::DS3225>{PWM::Timer::Timer2, PWM::Channel::Channel2, 0};
  auto servo2 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel1, 0, true};
  auto servo3 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel3, 5};
  static auto servoController = ServoController{servo1, servo2, servo3};
  servoController.run();

  static auto task1 = Task<2000, void (*)()>{[]() {
    TaskUtils::delay(1000);
    auto armControl = ArmControl{
            servoController,
            {100, 100}
    };

    while (true) {
      servoController.setTarget(0, 90, 360);
      TaskUtils::delay(2000);
      servoController.setTarget(0, 45, 360);
      TaskUtils::delay(3000);
    }
  }};
  task1.run();
  TaskUtils::startScheduler();
}
