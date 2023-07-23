#pragma once

namespace Application::OS::TaskUtils {
  void dispose();
  void delay(int ticks);
  void startScheduler();
}// namespace Application::OS::TaskUtils