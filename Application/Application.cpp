#include "Application.h"

#include "Hardware/PWM.h"
#include "Task.h"

void Application_RunApplication() {
  using namespace Application;
  using namespace Application::Hardware;

  auto task1 =
          Task{[](void *param) {
                 double count = 0;
                 int up = 1;
                 double minPeriod = (0.43 / 20.0);
                 double maxPeriod = (2.4 / 20.0);
                 PWM::startPWM(
                         Application::Hardware::PWM::Timer::Timer2,
                         Application::Hardware::PWM::Channel::Channel1
                 );
                 while (true) {
                   auto x = ((count * (maxPeriod - minPeriod)) / 180.0) + minPeriod;
                   PWM::setDutyCycle(PWM::Timer::Timer2, PWM::Channel::Channel1, x);
                   vTaskDelay(10);
                   count += up;
                   if (count > 180) up = -1;
                   else if (count < 0)
                     up = 1;
                 }
               },
               "task1",
               nullptr};
  task1.run();

  vTaskStartScheduler();
}
