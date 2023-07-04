#include "Application.h"

#include "Hardware/PWM.h"
#include "Hardware/Servo.h"
#include "Task.h"

static struct ServoInfo {
  double currentAngle{};
} servoInfo{};

void Application_RunApplication() {
  using namespace Application;
  using namespace Application::Hardware;

  auto task1 =
          Task{[](void *param) {
                 auto *servoParam = static_cast<ServoInfo *>(param);
                 if (servoParam == nullptr) Task::dispose();
                 double &count = servoParam->currentAngle;
                 int up = 1;
                 auto dutyCycleRange = Servo::DutyCycleRange{0.43 / 20.0, 2.4 / 20.0};
                 auto servo = Servo{PWM::Timer::Timer2, PWM::Channel::Channel1, dutyCycleRange};
                 while (true) {
                   servo.moveTo(count);
                   Task::delay(10);
                   count += up;
                   if (count > 180) up = -1;
                   else if (count < 0)
                     up = 1;
                 }
                 Task::dispose();
               },
               "task1",
               nullptr};
  task1.setTaskParameters(&servoInfo);
  task1.run();
  auto task2 = Task(
          [](void *param) {
            double count = 0;
            int up = 1;
            auto dutyCycleRange = Servo::DutyCycleRange{0.43 / 20.0, 2.4 / 20.0};
            auto servo = Servo{PWM::Timer::Timer2, PWM::Channel::Channel2, dutyCycleRange};
            while (true) {
              servo.moveTo(count);
              Task::delay(5);
              count += up;
              if (count > 180) up = -1;
              else if (count < 0)
                up = 1;
            }
            Task::dispose();
          },
          "task2"
  );
  task2.setTaskParameters(&servoInfo);
  task2.run();
  Task::startScheduler();
}
