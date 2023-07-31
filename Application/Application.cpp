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

  auto servo1 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel2, -5};
  auto servo2 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel1, 0, true};
  auto servo3 = ServoData<ServoTypes::SG90>{PWM::Timer::Timer2, PWM::Channel::Channel3, 5};
  static auto controller = ServoController{servo1, servo2, servo3};
  controller.run();

  static auto task1 = Task<2000, void (*)()>{[]() {
    TaskUtils::delay(1000);
    //    controller.setTarget(0, 90, 45);
    //    controller.setTarget(1, 90, 45);
    controller.setTarget(2, 45, (90 - 45) / 0.25);
    //    auto armControl = ArmControl{
    //            controller,
    //            {100, 100}
    //    };

    double c{};
    double i{20};
    while (true) {
      //      c += i;
      //      if (c > 100 || c < -100) {
      //        i = -i;
      //      }
      //      auto targetPosition = std::array{c, 150., 3.14 / 2.};
      //      armControl.positionTo({c, 500., 3.14 / 2.});
      TaskUtils::delay(100);
    }
  }};
  task1.run();
  TaskUtils::startScheduler();
}
