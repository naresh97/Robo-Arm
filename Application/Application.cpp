#include "Application.h"

#include "Hardware/PWM.h"
#include "Hardware/Servo.h"
#include "InverseKinematics/Positioning.h"
#include "Task.h"

struct Task1Parameters {
  std::array<double, Application::Kinematics::EffectorDimensions> targetPosition{};
} task1Parameters{};

void Application_RunApplication() {
  using namespace Application;
  using namespace Application::Hardware;

  auto task1 =
          Task{[](void *param) {
                 auto dutyCycleRange = Servo::DutyCycleRange{0.43 / 20.0, 2.4 / 20.0};

                 auto servo1 = Servo{PWM::Timer::Timer2, PWM::Channel::Channel1, dutyCycleRange};
                 auto servo2 = Servo{PWM::Timer::Timer2, PWM::Channel::Channel2, dutyCycleRange};
                 auto servo3 = Servo{PWM::Timer::Timer2, PWM::Channel::Channel3, dutyCycleRange};
                 Kinematics::Positioning positioning{{100, 100}};
                 while (true) {
                   task1Parameters.targetPosition[0] += 0;
                   task1Parameters.targetPosition[1] += 20;
                   task1Parameters.targetPosition[2] += 0;
                   task1Parameters.targetPosition[0] = task1Parameters.targetPosition[0] > 1000
                                                               ? 0
                                                               : task1Parameters.targetPosition[0];
                   task1Parameters.targetPosition[1] = task1Parameters.targetPosition[0] > 1000
                                                               ? 0
                                                               : task1Parameters.targetPosition[1];
                   task1Parameters.targetPosition[2] = task1Parameters.targetPosition[0] > 180
                                                               ? 0
                                                               : task1Parameters.targetPosition[2];

                   std::array<double, Kinematics::DegreesOfFreedom> x = {
                           servo1.getCurrentAngle(),
                           servo2.getCurrentAngle(),
                           servo3.getCurrentAngle()};
                   Kinematics::Positioning::convertToWorldAngles(x);
                   positioning.updateActuatorPositions(x);
                   positioning.updateDesiredEffectorPosition(task1Parameters.targetPosition);
                   auto newX = positioning.positionEffector();
                   Kinematics::Positioning::convertToToolAngles(newX);
                   servo1.moveTo(newX[0], 180);
                   servo2.moveTo(newX[1], 180);
                   servo3.moveTo(newX[2], 180);
                   Task::delay(1000);
                 }
                 Task::dispose();
               },
               "task1",
               nullptr};
  task1.run();

  Task::startScheduler();
}
